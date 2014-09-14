#include "stdafx.h"
#include "core_code.h"
#include<fstream>
using namespace std;
fstream m_file;


core_code::core_code(void)
{
	 pDSC=NULL;
	 ppDSCB8=NULL;
	 wavedata=NULL;
	 datalength=0;
	 g_dwCaptureBufferSize=0;

	 g_dwNextCaptureOffset=0;
	 time_adjusted=0;
}


core_code:: ~core_code(void)
{
	m_pEnumerator->Release();
	m_pDevice->Release();
    m_pEndptVolCtrl->Release();
	CoUninitialize();
}
HRESULT core_code:: SetCaptureNotifications(LPDIRECTSOUNDCAPTUREBUFFER8 pDSCB){ //事件通知，此处未用，用TIMER
	const int cEvents=3;
	LPDIRECTSOUNDNOTIFY8 pDSNotify;
	WAVEFORMATEX wfx;
	HANDLE rghEvent[cEvents]={0};
	DSBPOSITIONNOTIFY rgdsbpn[cEvents];
	HRESULT hr;
	if(NULL==pDSCB) return E_INVALIDARG;
	if (FAILED(hr=pDSCB->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&pDSNotify))){
		return hr;
	}
	if(FAILED(hr=pDSCB->GetFormat(&wfx,sizeof(WAVEFORMATEX),NULL))){
		return hr;
	}
	//create events
	for(int i=0;i<cEvents;i++){
		rghEvent[i]=CreateEvent(NULL,TRUE,FALSE,NULL);
		if(NULL==rghEvent[i]){
			hr=GetLastError();
			return hr;
		}
	}
	//describe notifications
	rgdsbpn[0].dwOffset=(wfx.nSamplesPerSec/2)-1;
	rgdsbpn[0].hEventNotify=rghEvent[0];
	
	rgdsbpn[1].dwOffset=wfx.nSamplesPerSec-1;
	rgdsbpn[1].hEventNotify=rghEvent[1];
	
	rgdsbpn[2].dwOffset=DSBPN_OFFSETSTOP;
	rgdsbpn[2].hEventNotify=rghEvent[2];
	
	hr=pDSNotify->SetNotificationPositions(cEvents,rgdsbpn);
	pDSNotify->Release();
	return hr;
}

HRESULT core_code:: CreateCaptureBuffer(LPDIRECTSOUNDCAPTURE8 pDSC, LPDIRECTSOUNDCAPTUREBUFFER8* ppDSCB8)
{
   HRESULT hr;
   DSCBUFFERDESC                dscbd;
   LPDIRECTSOUNDCAPTUREBUFFER   pDSCB;
   WAVEFORMATEX                 wfx =
      {WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0};
    //  {WAVE_FORMAT_PCM, 2, 22050, 88200, 4, 16, 0};
     // {WAVE_FORMAT_PCM, 2, 44100, 176400, 4, 16, 0};
     // wFormatTag, nChannels, nSamplesPerSec, mAvgBytesPerSec,
     // nBlockAlign, wBitsPerSample, cbSize

   if ((NULL == pDSC) || (NULL == ppDSCB8)) return E_INVALIDARG;
   dscbd.dwSize = sizeof(DSCBUFFERDESC);
   dscbd.dwFlags = 0;
   dscbd.dwBufferBytes =   wfx.nAvgBytesPerSec*1;      //*******保存多少秒的数据 就设置这里为多少乘以 wfx.nAvgBytesPerSec*********
   dscbd.dwReserved = 0;
   dscbd.lpwfxFormat = &wfx;
   dscbd.dwFXCount = 0;
   dscbd.lpDSCFXDesc = NULL;
   g_dwCaptureBufferSize=dscbd.dwBufferBytes; //set the value of  the globle variable

   wavedata=new char[g_dwCaptureBufferSize];  //wavadata 给该指针分配空间
   memset(wavedata,0,g_dwCaptureBufferSize);  //全部置零
   if (SUCCEEDED(hr = pDSC->CreateCaptureBuffer(&dscbd, &pDSCB, NULL)))
   {
     hr = pDSCB->QueryInterface(IID_IDirectSoundCaptureBuffer8, (LPVOID*)ppDSCB8);
     pDSCB->Release();  
   }
   return hr;
} 

HRESULT core_code:: RecordCaptureData(){
	LPDIRECTSOUNDCAPTUREBUFFER8 g_pDSBCapture=ppDSCB8;

	HRESULT hr;
	VOID* pbCaptureData=NULL;
	DWORD dwCaptureLength;
	VOID* pbCaptureData2=NULL;
	DWORD dwCaptureLength2;
	VOID* pbPlayData=NULL;
//	UINT dwDataWrote;
	DWORD dwReadPos;
	LONG lLockSize;

	

	if(FAILED(hr=g_pDSBCapture->GetCurrentPosition(NULL,&dwReadPos)))
		return hr;
	lLockSize=dwReadPos-g_dwNextCaptureOffset;
	if( lLockSize<0) lLockSize+=g_dwCaptureBufferSize;
	if( lLockSize==0) return S_FALSE;
	if(FAILED(hr=g_pDSBCapture->Lock(g_dwNextCaptureOffset,lLockSize,&pbCaptureData,&dwCaptureLength,&pbCaptureData2,&dwCaptureLength2,0L)))
		return hr;
	datalength=dwCaptureLength;//important
	//src:(byte*) pbCaptureData2  direction: **  length:dwCaptureLength2
	memcpy(wavedata,pbCaptureData,datalength);//copy wavedata to analyse amplitude,important
	//cout<<"the lengrh is "<<dwCaptureLength<<"  ***";
	//cout<<"the lengrh2 is"<<dwCaptureLength2<<"  ***";
	//unlock it
	g_pDSBCapture->Unlock(pbCaptureData,dwCaptureLength,pbCaptureData2,dwCaptureLength2);

	//move the capture offset forword
	g_dwNextCaptureOffset+=dwCaptureLength;
	g_dwNextCaptureOffset%=g_dwCaptureBufferSize;
	g_dwNextCaptureOffset+=dwCaptureLength2;
	g_dwNextCaptureOffset%=g_dwCaptureBufferSize;
	
	return S_OK;
}



bool core_code:: capture_initialize(){
	if(FAILED(hr=DirectSoundCaptureCreate8(NULL ,&pDSC,NULL)))
		//cout<<"create capture object failed\n";
	
	dsccaps.dwSize=sizeof(DSCCAPS);
	pDSC->GetCaps(&dsccaps);
	//wcout<<dsccaps.dwFlags<< "  "<<dsccaps.dwFormats<<  "  "<<dsccaps. dwChannels<<endl;
	//Multiple capture objects can be used simultaneously on the capture device. 
	
	
	if(FAILED(hr=CreateCaptureBuffer(pDSC,&ppDSCB8)))
		//cout<<"CreateCaptureBuffer failed \n";
		return false;
	return true;
}

bool core_code:: capture_start(){
		if(FAILED(ppDSCB8->Start(DSCBSTART_LOOPING)))
			//cout<<"capture start failed\n";
		return false;
	return true;
}

 bool  core_code:: volume_control_intialize(){
	CoInitialize(NULL);
	CoCreateGuid(&m_guidMyContext);
	CoCreateInstance(__uuidof(MMDeviceEnumerator),
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IMMDeviceEnumerator),
		(void**)&m_pEnumerator);
	 m_pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &m_pDevice);
	 m_pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL, NULL, (void**)&m_pEndptVolCtrl);
	  m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&fVolume);
	  fVolume*=100;
	// cout<<"volume before is"<<fVolume<<"***";
	 return true;
 }

 bool core_code:: wavedata_collect( ){
	 if(FAILED(hr=RecordCaptureData()))
		// cout<<"record failed\n";
	 return false;
	return true;
 }
int core_code:: amplitude_analyse(){  //返回音量水平，0~124
	
	//float afactor=2*255.0/8;
	long amp,amp_average;
	DWORD i,j=0;
	if(datalength==0)
		datalength=1;

	for(i=0;i<datalength;i++){
		//amp=(unsigned int)labs(((long)wavedata[i]-128L)/afactor);
	amp=labs((long)wavedata[i]+128L);// 静音约对应-128
	j+=amp;
	}
	amp_average=j/datalength;
	amp_average-=25;//再次调整
	return amp_average;
}

void core_code:: volume_control(float set_volume_value){
	
	 m_pEndptVolCtrl->SetMasterVolumeLevelScalar( set_volume_value/100, &m_guidMyContext);
	 m_pEndptVolCtrl->GetMasterVolumeLevelScalar(&fVolume);
	 fVolume*=100;
	// cout<<"volume now is"<<fVolume<<"***";
}

