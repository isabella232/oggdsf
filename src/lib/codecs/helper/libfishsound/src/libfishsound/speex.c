/*
   Copyright (C) 2003 Commonwealth Scientific and Industrial Research
   Organisation (CSIRO) Australia

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

   - Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

   - Neither the name of CSIRO Australia nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
   PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE ORGANISATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ctype.h>

#include "private.h"

/*#define DEBUG*/

#if HAVE_SPEEX

#include <speex.h>
#include <speex_header.h>
#include <speex_stereo.h>
#include <speex_callbacks.h>

#define DEFAULT_ENH_ENABLED 1

#define MAX_FRAME_BYTES 2000

//Zen's hack
__int64 frame_count = 0;
typedef struct _FishSoundSpeexEnc {
  int frame_offset; /* number of speex frames done in this packet */
  int pcm_offset;
  char cbits[MAX_FRAME_BYTES];
  int id;
} FishSoundSpeexEnc;

typedef struct _FishSoundSpeexInfo {
  int packetno;
  void * st;
  SpeexBits bits;
  int frame_size;
  int nframes;
  int extra_headers;
  SpeexStereoState stereo;
  int pcm_len; /* nr frames in pcm */
  float * ipcm; /* interleaved pcm */
  float * pcm[2]; /* Speex does max 2 channels */
  FishSoundSpeexEnc * enc;
} FishSoundSpeexInfo;

static int
fs_speex_identify (unsigned char * buf, long bytes)
{
  SpeexHeader * header;

  if (bytes < 8) return FISH_SOUND_UNKNOWN;

  if (!strncmp (buf, "Speex   ", 8)) {
    /* if only a short buffer was passed, do a weak identify */
    if (bytes == 8) return FISH_SOUND_SPEEX;

    /* otherwise, assume the buffer is an entire initial header and
     * feed it to speex_packet_to_header() */
    if ((header = speex_packet_to_header ((char *)buf, (int)bytes)) != NULL) {
      free(header);
      return FISH_SOUND_SPEEX;
    }
  }

  return FISH_SOUND_UNKNOWN;
}

static int
fs_speex_command (FishSound * fsound, int command, void * data, int datasize)
{
	if (command == SPEEX_TELL_GRANULE_POS) {
		__int64* locTemp = (__int64*) data;
		*locTemp = frame_count;
			
	}
	return 0;

}

#ifdef FS_DECODE
static void *
process_header(unsigned char * buf, long bytes, int enh_enabled,
	       int * frame_size, int * rate,
	       int * nframes, int forceMode, int * channels,
	       SpeexStereoState * stereo, int * extra_headers)
{
  void *st;
  SpeexMode *mode;
  SpeexHeader *header;
  int modeID;
  SpeexCallback callback;

  header = speex_packet_to_header((char*)buf, (int)bytes);
  if (!header) {
    /*info_dialog_new ("Speex error", NULL, "Speex: cannot read header");*/
    return NULL;
  }

  if (header->mode >= SPEEX_NB_MODES) {
    /*
    info_dialog_new ("Speex error", NULL,
		     "Mode number %d does not (any longer) exist in this version\n",
		     header->mode);
    */
    return NULL;
  }

  modeID = header->mode;
  if (forceMode!=-1)
    modeID = forceMode;
  mode = speex_mode_list[modeID];

  if (header->speex_version_id > 1) {
    /*
    info_dialog_new ("Speex error", NULL,
		     "This file was encoded with Speex bit-stream version %d, "
		     "which I don't know how to decode\n",
		     header->speex_version_id);
    */
    return NULL;
  }

  if (mode->bitstream_version < header->mode_bitstream_version) {
    /*
    info_dialog_new ("Speex error", NULL,
		     "The file was encoded with a newer version of Speex. "
		     "You need to upgrade in order to play it.\n");
    */
    return NULL;
  }

  if (mode->bitstream_version > header->mode_bitstream_version) {
    /*
    info_dialog_new ("Speex error", NULL,
		     "The file was encoded with an older version of Speex. "
		     "You would need to downgrade the version in order to play it.\n");
    */
    return NULL;
  }

  st = speex_decoder_init(mode);
  if (!st) {
    /*
      info_dialog_new ("Speex error", NULL,
      "Decoder initialization failed.\n");
    */
    return NULL;
  }

  speex_decoder_ctl(st, SPEEX_SET_ENH, &enh_enabled);
  speex_decoder_ctl(st, SPEEX_GET_FRAME_SIZE, frame_size);

  if (!(*channels==1))
    {
      callback.callback_id = SPEEX_INBAND_STEREO;
      callback.func = speex_std_stereo_request_handler;
      callback.data = stereo;
      speex_decoder_ctl(st, SPEEX_SET_HANDLER, &callback);
    }
  if (!*rate)
    *rate = header->rate;
  /* Adjust rate if --force-* options are used */
  if (forceMode!=-1)
    {
      if (header->mode < forceMode)
	*rate <<= (forceMode - header->mode);
      if (header->mode > forceMode)
	*rate >>= (header->mode - forceMode);
    }

  speex_decoder_ctl(st, SPEEX_SET_SAMPLING_RATE, rate);

  *nframes = header->frames_per_packet;

  if (*channels == -1)
    *channels = header->nb_channels;

#ifdef DEBUG  
  fprintf (stderr, "Decoding %d Hz audio using %s mode",
	   *rate, mode->modeName);

  if (*channels==1)
      fprintf (stderr, " (mono");
   else
      fprintf (stderr, " (stereo");

  if (header->vbr)
    fprintf (stderr, " (VBR)\n");
  else
    fprintf(stderr, "\n");
#endif

  *extra_headers = header->extra_headers;

  free(header);

  return st;
}


static long
fs_speex_decode (FishSound * fsound, unsigned char * buf, long bytes)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  int enh_enabled = DEFAULT_ENH_ENABLED;
  int rate = 0;
  int channels = -1;
  int forceMode = -1;
  int i, j;
  float ** retpcm;

  if (fss->packetno == 0) {
    fss->st = process_header (buf, bytes, enh_enabled,
			      &fss->frame_size, &rate,
			      &fss->nframes, forceMode, &channels,
			      &fss->stereo,
			      &fss->extra_headers);

    if (fss->st == NULL) {
      /* XXX: error */
    }

#ifdef DEBUG
    printf ("speex: got %d channels, %d Hz\n", channels, rate);
#endif

    fsound->info.samplerate = rate;
    fsound->info.channels = channels;

    fss->ipcm = malloc (sizeof (float) * fss->frame_size * channels);

    if (channels == 1) {
      fss->pcm[0] = fss->ipcm;
    } else if (channels == 2) {
      fss->pcm[0] = malloc (sizeof (float) * fss->frame_size);
      fss->pcm[1] = malloc (sizeof (float) * fss->frame_size);
    }

    if (fss->nframes == 0) fss->nframes = 1;

  } else if (fss->packetno <= 1+fss->extra_headers) {
    /* XXX: metadata etc. */
  } else {
    speex_bits_read_from (&fss->bits, (char *)buf, (int)bytes);

    for (i = 0; i < fss->nframes; i++) {
      /* Decode frame */
      speex_decode (fss->st, &fss->bits, fss->ipcm);

      if (fsound->info.channels == 2) {
	speex_decode_stereo (fss->ipcm, fss->frame_size, &fss->stereo);
	if (fsound->interleave) {
	  for (j = 0; j < fss->frame_size * fsound->info.channels; j++) {
	    fss->ipcm[j] /= 32767.0;
	  }
	} else {
	  _fs_deinterleave ((float **)fss->ipcm, fss->pcm,
			    fss->frame_size, 2, (float)(1/32767.0));
	}
      } else {
	for (j = 0; j < fss->frame_size; j++) {
	  fss->ipcm[j] /= 32767.0;
	}
      }

      if (fsound->interleave) {
	retpcm = (float **)fss->ipcm;
      } else {
	retpcm = (float **)fss->pcm;
      }

      /* fss->pcm is ready to go! */
      if (fsound->callback) {
	((FishSoundDecoded)fsound->callback) (fsound, retpcm,
					      fss->frame_size,
					      fsound->user_data);
      }
    }
  }

  fss->packetno++;

  return 0;
}
#else /* !FS_DECODE */

#define fs_speex_decode NULL

#endif


#ifdef FS_ENCODE
static FishSound *
fs_speex_enc_headers (FishSound * fsound)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  SpeexMode * mode = NULL;
  SpeexHeader header;
  char * buf;
  int bytes;

  /* XXX: set wb, nb, uwb modes */
  mode = &speex_wb_mode;

  speex_init_header (&header, fsound->info.samplerate, 1, mode);
  header.frames_per_packet = fss->nframes; /* XXX: frames per packet */
  header.vbr = 1; /* XXX: VBR */
  header.nb_channels = fsound->info.channels;

  fss->st = speex_encoder_init (mode);

  if (fsound->callback) {
    FishSoundEncoded encoded = (FishSoundEncoded)fsound->callback;

    /* header */
    buf = speex_header_to_packet (&header, &bytes);    
    encoded (fsound, (unsigned char *)buf, (long)bytes, fsound->user_data);
    fss->packetno++;
    free (buf);

    /* XXX: and comments */
    encoded (fsound, NULL, 0, fsound->user_data);
  }

  speex_encoder_ctl (fss->st, SPEEX_SET_SAMPLING_RATE,
		     &fsound->info.samplerate);

  speex_encoder_ctl (fss->st, SPEEX_GET_FRAME_SIZE, &fss->frame_size);

#ifdef DEBUG
  printf ("got frame size %d\n", fss->frame_size);
#endif

  /* XXX: blah blah blah ... set VBR etc. */

  fss->ipcm = malloc (fss->frame_size * fsound->info.channels
		      * sizeof (float));

  return fsound;
}

static long
fs_speex_encode_write (FishSound * fsound)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse = (FishSoundSpeexEnc *)fss->enc;
  int bytes;

  bytes = speex_bits_write (&fss->bits, fse->cbits, MAX_FRAME_BYTES);
  speex_bits_reset (&fss->bits);

  if (fsound->callback) {
    FishSoundEncoded encoded = (FishSoundEncoded)fsound->callback;

    encoded (fsound, (unsigned char *)fse->cbits, (long)bytes,
	     fsound->user_data);
  }

  return bytes;
}

static long
fs_speex_encode_block (FishSound * fsound)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse = (FishSoundSpeexEnc *)fss->enc;
  long nencoded = 0;

  if (fsound->info.channels == 2)
    speex_encode_stereo (fss->ipcm, fse->pcm_offset, &fss->bits);
  
  speex_encode (fss->st, fss->ipcm, &fss->bits);
  
  fse->frame_offset++;
  if (fse->frame_offset == fss->nframes) {
    nencoded = fs_speex_encode_write (fsound);
    fse->frame_offset = 0;
  }

  fse->pcm_offset = 0;

  return nencoded;
}

static long
fs_speex_encode_i (FishSound * fsound, float ** pcm, long frames)
{
  
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse = (FishSoundSpeexEnc *)fss->enc;
  long remaining = frames, len, nencoded = 0;
  int j, start, end;
  int channels = fsound->info.channels;
  float * p = (float *)pcm;

  

  if (fss->packetno == 0)
    fs_speex_enc_headers (fsound);

  while (remaining > 0) {
    len = MIN (remaining, fss->frame_size - fse->pcm_offset);
	
	//Zen's hack
	frame_count+= len;
	//
    start = fse->pcm_offset * channels;
    end = (len + fse->pcm_offset) * channels;
    for (j = start; j < end; j++) {
      fss->ipcm[j] = *p++ * (float)32767.0;
    }

    fse->pcm_offset += len;

    if (fse->pcm_offset == fss->frame_size) {
#if 0
      fse->pcm_offset = 0;

      if (fsound->info.channels == 2)
	speex_encode_stereo (fss->ipcm, fss->frame_size, &fss->bits);
      
      speex_encode (fss->st, fss->ipcm, &fss->bits);
    
      fse->frame_offset++;
      if (fse->frame_offset == fss->nframes) {
	nencoded += fs_speex_encode_write (fsound);
	fse->frame_offset = 0;
      }
#else
      nencoded += fs_speex_encode_block (fsound);
#endif
    }

    remaining -= len;
  }

  return nencoded;
}

static long
fs_speex_encode_n (FishSound * fsound, float * pcm[], long frames)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse = (FishSoundSpeexEnc *)fss->enc;
  long remaining = frames, len;
  int i, j;

  if (fss->packetno == 0)
    fs_speex_enc_headers (fsound);

  fss->pcm[0] = pcm[0];
  if (fsound->info.channels == 2)
    fss->pcm[1] = pcm[1];

  for (i = 0; i < fss->nframes; i++) {
    if (remaining > 0) {
      len = MIN (remaining, fss->frame_size);

      if (fsound->info.channels == 2) {
	_fs_interleave (fss->pcm, (float **)fss->ipcm,
			fss->frame_size, 2, 32767.0);
	speex_encode_stereo (fss->ipcm, len, &fss->bits);
      } else {
	for (j = 0; j < len; j++) {
	  fss->ipcm[j] = fss->pcm[0][j] * (float)32767.0;
	}
      }

      speex_encode (fss->st, fss->ipcm, &fss->bits);

      fss->pcm[0] += len;
      if (fsound->info.channels == 2)
	fss->pcm[1] += len;
      
      remaining -= len;
    } else {
      speex_bits_pack (&fss->bits, 15, 5);
    }
  }

  return fs_speex_encode_write (fsound);
}

static long
fs_speex_flush (FishSound * fsound)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse = (FishSoundSpeexEnc *)fss->enc;
  long nencoded = 0;

  if (fse->pcm_offset > 0) {
#if 0
    fse->pcm_offset = 0;

    if (fsound->info.channels == 2)
      speex_encode_stereo (fss->ipcm, fse->pcm_offset, &fss->bits);
      
    speex_encode (fss->st, fss->ipcm, &fss->bits);
    
    fse->frame_offset++;
    if (fse->frame_offset == fss->nframes) {
      nencoded += fs_speex_encode_write (fsound);
      fse->frame_offset = 0;
    }
#else
    nencoded += fs_speex_encode_block (fsound);
#endif
  }

  if (fse->frame_offset == 0) return 0;

  while (fse->frame_offset < fss->nframes) {
    speex_bits_pack (&fss->bits, 15, 5);
    fse->frame_offset++;
  }

  nencoded += fs_speex_encode_write (fsound);
  fse->frame_offset = 0;

  return nencoded;
}

#else /* !FS_ENCODE */

#define fs_speex_encode_i NULL
#define fs_speex_encode_n NULL
#define fs_speex_flush NULL

#endif

static int
fs_speex_reset (FishSound * fsound)
{
  /*FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;*/

  return 0;
}

static FishSound *
fs_speex_enc_init (FishSound * fsound)
{
  
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;
  FishSoundSpeexEnc * fse;

  fse = malloc (sizeof (FishSoundSpeexEnc));
  if (fse == NULL) return NULL;
  //Zen's hack
  frame_count = 0;
  //

  fse->frame_offset = 0;
  fse->pcm_offset = 0;
  fse->id = 0;

  fss->enc = fse;

  return fsound;
}

static FishSound *
fs_speex_init (FishSound * fsound)
{
  FishSoundSpeexInfo * fss;
  SpeexStereoState stereo_init = SPEEX_STEREO_STATE_INIT;

  fss = malloc (sizeof (FishSoundSpeexInfo));
  if (fss == NULL) return NULL;

  fss->packetno = 0;
  fss->st = NULL;
  fss->frame_size = 0;
  fss->nframes = 1;
  fss->pcm_len = 0;
  fss->ipcm = NULL;
  fss->pcm[0] = NULL;
  fss->pcm[1] = NULL;

  memcpy (&fss->stereo, &stereo_init, sizeof (SpeexStereoState));

  speex_bits_init (&fss->bits);

  fsound->codec_data = fss;

  if (fsound->mode == FISH_SOUND_ENCODE)
    fs_speex_enc_init (fsound);

  return fsound;
}

static FishSound *
fs_speex_delete (FishSound * fsound)
{
  FishSoundSpeexInfo * fss = (FishSoundSpeexInfo *)fsound->codec_data;

  if (fsound->mode == FISH_SOUND_DECODE) {
    speex_decoder_destroy (fss->st);
  } else if (fsound->mode == FISH_SOUND_ENCODE) {
    speex_encoder_destroy (fss->st);
  }
  speex_bits_destroy (&fss->bits);

  free (fss);
  fsound->codec_data = NULL;

  return fsound;
}

static FishSoundFormat fs_speex_format = {
  FISH_SOUND_SPEEX,
  "Speex (Xiph.Org)",
  "spx"
};

FishSoundCodec fish_sound_speex = {
  &fs_speex_format,
  fs_speex_identify,
  fs_speex_init,
  fs_speex_delete,
  fs_speex_reset,
  fs_speex_command,
  fs_speex_decode,
  fs_speex_encode_i,
  fs_speex_encode_n,
  fs_speex_flush
};

#else /* !HAVE_SPEEX */

static int
fs_nospeex_identify (unsigned char * buf, long bytes)
{
  return FISH_SOUND_UNKNOWN;
}

FishSoundCodec fish_sound_speex = {
  NULL, /* format */
  fs_nospeex_identify, /* identify */
  NULL, /* init */
  NULL, /* delete */
  NULL, /* reset */
  NULL, /* command */
  NULL, /* decode */
  NULL, /* encode_i */
  NULL, /* encode_n */
  NULL  /* flush */
};

#endif

