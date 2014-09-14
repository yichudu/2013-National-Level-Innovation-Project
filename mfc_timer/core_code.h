
#pragma once
# include "stdafx.h"





class core_code
{
public:
	//以下用于录音
LPDIRECTSOUNDCAPTURE8 pDSC;
LPDIRECTSOUNDCAPTUREBUFFER8 ppDSCB8;
DSCCAPS dsccaps;
HRESULT hr;
char* wavedata;
DWORD datalength;
DWORD g_dwCaptureBufferSize;

DWORD g_dwNextCaptureOffset;


//以下获取并调整系统音量
GUID m_guidMyContext;
IMMDeviceEnumerator *m_pEnumerator;
IMMDevice *m_pDevice;
IAudioEndpointVolume *m_pEndptVolCtrl;

float fVolume;
int latest_amplitude,critical_level;
float set_volume_value;
int time_adjusted;

	//function
     core_code(void);
	~core_code(void);
HRESULT CreateCaptureBuffer(LPDIRECTSOUNDCAPTURE8 pDSC,LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCB8);
HRESULT RecordCaptureData();
HRESULT SetCaptureNotifications(LPDIRECTSOUNDCAPTUREBUFFER8 pDSCB);//not useful
bool capture_initialize();
bool volume_control_intialize();
bool capture_start();
bool wavedata_collect( );
int amplitude_analyse();
void volume_control(float set_volume_value);

};

//extern core_code ccode;