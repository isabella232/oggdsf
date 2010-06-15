// Copyright (c) 2010 The WebM project authors. All Rights Reserved.
//
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file in the root of the source
// tree. An additional intellectual property rights grant can be found
// in the file PATENTS.  All contributing project authors may
// be found in the AUTHORS file in the root of the source tree.

#pragma once
#include <strmif.h>
#include <string>
#include "clockable.hpp"
#include "vp8encoderinpin.hpp"
#include "vp8encoderoutpinvideo.hpp"
#include "vp8encoderoutpinpreview.hpp"
#include "vp8encoderidl.h"

namespace VP8EncoderLib
{

class Filter : public IBaseFilter,
               public IVP8Encoder,
               public CLockable
{
    friend HRESULT CreateInstance(
            IClassFactory*,
            IUnknown*, 
            const IID&, 
            void**);
    
    Filter(IClassFactory*, IUnknown*);
    virtual ~Filter();
    
    Filter(const Filter&);
    Filter& operator=(const Filter&);
    
public:

    //IUnknown

    HRESULT STDMETHODCALLTYPE QueryInterface(const IID&, void**);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    
    //IBaseFilter

    HRESULT STDMETHODCALLTYPE GetClassID(CLSID*);    
    HRESULT STDMETHODCALLTYPE Stop();    
    HRESULT STDMETHODCALLTYPE Pause();
    HRESULT STDMETHODCALLTYPE Run(REFERENCE_TIME);    
    HRESULT STDMETHODCALLTYPE GetState(DWORD, FILTER_STATE*);    
    HRESULT STDMETHODCALLTYPE SetSyncSource(IReferenceClock*);
    HRESULT STDMETHODCALLTYPE GetSyncSource(IReferenceClock**);
    HRESULT STDMETHODCALLTYPE EnumPins(IEnumPins**);    
    HRESULT STDMETHODCALLTYPE FindPin(LPCWSTR, IPin**);    
    HRESULT STDMETHODCALLTYPE QueryFilterInfo(FILTER_INFO*);    
    HRESULT STDMETHODCALLTYPE JoinFilterGraph(IFilterGraph*, LPCWSTR);    
    HRESULT STDMETHODCALLTYPE QueryVendorInfo(LPWSTR*);


    //IVP8Encoder
    
    HRESULT STDMETHODCALLTYPE ApplySettings();    
    HRESULT STDMETHODCALLTYPE ResetSettings();

    HRESULT STDMETHODCALLTYPE SetDeadline(int);
    HRESULT STDMETHODCALLTYPE GetDeadline(int*);
    
    HRESULT STDMETHODCALLTYPE SetThreadCount(int); 
    HRESULT STDMETHODCALLTYPE GetThreadCount(int*);
    
    HRESULT STDMETHODCALLTYPE SetErrorResilient(int);
    HRESULT STDMETHODCALLTYPE GetErrorResilient(int*);
    
    HRESULT STDMETHODCALLTYPE SetDropframeThreshold(int);    
    HRESULT STDMETHODCALLTYPE GetDropframeThreshold(int*);
    
    HRESULT STDMETHODCALLTYPE SetResizeAllowed(int); 
    HRESULT STDMETHODCALLTYPE GetResizeAllowed(int*); 
    
    HRESULT STDMETHODCALLTYPE SetResizeUpThreshold(int); 
    HRESULT STDMETHODCALLTYPE GetResizeUpThreshold(int*);
    
    HRESULT STDMETHODCALLTYPE SetResizeDownThreshold(int); 
    HRESULT STDMETHODCALLTYPE GetResizeDownThreshold(int*);        

    HRESULT STDMETHODCALLTYPE SetEndUsage(VP8EndUsage);    
    HRESULT STDMETHODCALLTYPE GetEndUsage(VP8EndUsage*);
    
    HRESULT STDMETHODCALLTYPE SetLagInFrames(int);    
    HRESULT STDMETHODCALLTYPE GetLagInFrames(int*); 
    
    HRESULT STDMETHODCALLTYPE SetTokenPartitions(int);
    HRESULT STDMETHODCALLTYPE GetTokenPartitions(int*);

    HRESULT STDMETHODCALLTYPE SetTargetBitrate(int);    
    HRESULT STDMETHODCALLTYPE GetTargetBitrate(int*);
    
    HRESULT STDMETHODCALLTYPE SetMinQuantizer(int);    
    HRESULT STDMETHODCALLTYPE GetMinQuantizer(int*);
    
    HRESULT STDMETHODCALLTYPE SetMaxQuantizer(int);    
    HRESULT STDMETHODCALLTYPE GetMaxQuantizer(int*);
    
    HRESULT STDMETHODCALLTYPE SetUndershootPct(int);    
    HRESULT STDMETHODCALLTYPE GetUndershootPct(int*);
    
    HRESULT STDMETHODCALLTYPE SetOvershootPct(int);    
    HRESULT STDMETHODCALLTYPE GetOvershootPct(int*);
    
    HRESULT STDMETHODCALLTYPE SetDecoderBufferSize(int);    
    HRESULT STDMETHODCALLTYPE GetDecoderBufferSize(int*);
    
    HRESULT STDMETHODCALLTYPE SetDecoderBufferInitialSize(int);    
    HRESULT STDMETHODCALLTYPE GetDecoderBufferInitialSize(int*);
    
    HRESULT STDMETHODCALLTYPE SetDecoderBufferOptimalSize(int);    
    HRESULT STDMETHODCALLTYPE GetDecoderBufferOptimalSize(int*);
    
    HRESULT STDMETHODCALLTYPE SetKeyframeMode(VP8KeyframeMode);    
    HRESULT STDMETHODCALLTYPE GetKeyframeMode(VP8KeyframeMode*);
    
    HRESULT STDMETHODCALLTYPE SetKeyframeMinInterval(int);    
    HRESULT STDMETHODCALLTYPE GetKeyframeMinInterval(int*);
    
    HRESULT STDMETHODCALLTYPE SetKeyframeMaxInterval(int);    
    HRESULT STDMETHODCALLTYPE GetKeyframeMaxInterval(int*); 
            
#if 0
    HRESULT STDMETHODCALLTYPE SetPassMode(VP8PassMode);
    HRESULT STDMETHODCALLTYPE GetPassMode(VP8PassMode*);
    
    HRESULT STDMETHODCALLTYPE SetTwoPassStatsBuf(const BYTE*, LONGLONG);    
    HRESULT STDMETHODCALLTYPE GetTwoPassStatsBuf(const BYTE**, LONGLONG*);
#endif

private:
    class CNondelegating : public IUnknown
    {
        CNondelegating(const CNondelegating&);
        CNondelegating& operator=(const CNondelegating&);

    public:
    
        Filter* const m_pFilter;
        LONG m_cRef;
        
        explicit CNondelegating(Filter*);
        virtual ~CNondelegating();
        
        HRESULT STDMETHODCALLTYPE QueryInterface(const IID&, void**);
        ULONG STDMETHODCALLTYPE AddRef();
        ULONG STDMETHODCALLTYPE Release();

    };
    
    IClassFactory* const m_pClassFactory;
    CNondelegating m_nondelegating;
    IUnknown* const m_pOuter;  //decl must follow m_nondelegating
    REFERENCE_TIME m_start;
    IReferenceClock* m_clock;
    
public:
    FILTER_INFO m_info;
    FILTER_STATE m_state;
    Inpin m_inpin;
    OutpinVideo m_outpin_video;
    OutpinPreview m_outpin_preview;
    
    struct Config
    {
        typedef __int32 int32_t;
        
        int32_t deadline;
        
        //g_usage
        int32_t threads;
        //g_profile
        //g_w
        //g_h
        //g_timebase
        int32_t error_resilient;
        int32_t lag_in_frames; 
        int32_t dropframe_thresh;
        int32_t resize_allowed;
        int32_t resize_up_thresh;
        int32_t resize_down_thresh;        
        int32_t end_usage;

        //rc_twopass_stats_in        

        int32_t target_bitrate;
        int32_t min_quantizer;
        int32_t max_quantizer;
        int32_t undershoot_pct;
        int32_t overshoot_pct;
        int32_t decoder_buffer_size;
        int32_t decoder_buffer_initial_size;
        int32_t decoder_buffer_optimal_size;
        int32_t keyframe_mode;
        int32_t keyframe_min_interval;
        int32_t keyframe_max_interval;
        
        int32_t token_partitions;  

#if 0        
        int32_t pass_mode;
        const BYTE* two_pass_stats_buf;
        LONGLONG two_pass_stats_buflen;
#endif
        
        void Init();
    }; 
    
    Config m_cfg;          
    
private:
    HRESULT OnStart();
    void OnStop();

};


}  //end namespace VP8EncoderLib
