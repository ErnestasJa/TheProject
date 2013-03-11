
#ifndef glxw_egl_h
#define glxw_egl_h

struct glxw_egl;

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>


#ifndef __gl_h_
#define __gl_h_
#endif

#ifdef __cplusplus
extern "C" {
#endif

int glxwInitEGL(void);
int glxwInitEGLCtx(struct glxw_egl *ctx);

struct glxw_egl {
PFNEGLLOCKSURFACEKHRPROC _eglLockSurfaceKHR;
PFNEGLUNLOCKSURFACEKHRPROC _eglUnlockSurfaceKHR;
PFNEGLCREATEIMAGEKHRPROC _eglCreateImageKHR;
PFNEGLDESTROYIMAGEKHRPROC _eglDestroyImageKHR;
PFNEGLCREATESYNCKHRPROC _eglCreateSyncKHR;
PFNEGLDESTROYSYNCKHRPROC _eglDestroySyncKHR;
PFNEGLCLIENTWAITSYNCKHRPROC _eglClientWaitSyncKHR;
PFNEGLSIGNALSYNCKHRPROC _eglSignalSyncKHR;
PFNEGLGETSYNCATTRIBKHRPROC _eglGetSyncAttribKHR;
PFNEGLCREATEFENCESYNCNVPROC _eglCreateFenceSyncNV;
PFNEGLDESTROYSYNCNVPROC _eglDestroySyncNV;
PFNEGLFENCENVPROC _eglFenceNV;
PFNEGLCLIENTWAITSYNCNVPROC _eglClientWaitSyncNV;
PFNEGLSIGNALSYNCNVPROC _eglSignalSyncNV;
PFNEGLGETSYNCATTRIBNVPROC _eglGetSyncAttribNV;
PFNEGLCREATEPIXMAPSURFACEHIPROC _eglCreatePixmapSurfaceHI;
PFNEGLCREATEDRMIMAGEMESAPROC _eglCreateDRMImageMESA;
PFNEGLEXPORTDRMIMAGEMESAPROC _eglExportDRMImageMESA;
PFNEGLPOSTSUBBUFFERNVPROC _eglPostSubBufferNV;
PFNEGLGETSYSTEMTIMEFREQUENCYNVPROC _eglGetSystemTimeFrequencyNV;
PFNEGLGETSYSTEMTIMENVPROC _eglGetSystemTimeNV;
PFNEGLCREATESTREAMKHRPROC _eglCreateStreamKHR;
PFNEGLDESTROYSTREAMKHRPROC _eglDestroyStreamKHR;
PFNEGLSTREAMATTRIBKHRPROC _eglStreamAttribKHR;
PFNEGLQUERYSTREAMKHRPROC _eglQueryStreamKHR;
PFNEGLQUERYSTREAMU64KHRPROC _eglQueryStreamu64KHR;
PFNEGLSTREAMCONSUMERGLTEXTUREEXTERNALKHRPROC _eglStreamConsumerGLTextureExternalKHR;
PFNEGLSTREAMCONSUMERACQUIREKHRPROC _eglStreamConsumerAcquireKHR;
PFNEGLSTREAMCONSUMERRELEASEKHRPROC _eglStreamConsumerReleaseKHR;
PFNEGLCREATESTREAMPRODUCERSURFACEKHRPROC _eglCreateStreamProducerSurfaceKHR;
PFNEGLQUERYSTREAMTIMEKHRPROC _eglQueryStreamTimeKHR;
PFNEGLGETSTREAMFILEDESCRIPTORKHRPROC _eglGetStreamFileDescriptorKHR;
PFNEGLCREATESTREAMFROMFILEDESCRIPTORKHRPROC _eglCreateStreamFromFileDescriptorKHR;
PFNEGLWAITSYNCKHRPROC _eglWaitSyncKHR;
PFNEGLQUERYNATIVEDISPLAYNVPROC _eglQueryNativeDisplayNV;
PFNEGLQUERYNATIVEWINDOWNVPROC _eglQueryNativeWindowNV;
PFNEGLQUERYNATIVEPIXMAPNVPROC _eglQueryNativePixmapNV;
PFNEGLSETBLOBCACHEFUNCSANDROIDPROC _eglSetBlobCacheFuncsANDROID;
PFNEGLDUPNATIVEFENCEFDANDROIDPROC _eglDupNativeFenceFDANDROID;
};

extern struct glxw_egl *glxw_egl;

#define eglLockSurfaceKHR (glxw_egl->_eglLockSurfaceKHR)
#define eglUnlockSurfaceKHR (glxw_egl->_eglUnlockSurfaceKHR)
#define eglCreateImageKHR (glxw_egl->_eglCreateImageKHR)
#define eglDestroyImageKHR (glxw_egl->_eglDestroyImageKHR)
#define eglCreateSyncKHR (glxw_egl->_eglCreateSyncKHR)
#define eglDestroySyncKHR (glxw_egl->_eglDestroySyncKHR)
#define eglClientWaitSyncKHR (glxw_egl->_eglClientWaitSyncKHR)
#define eglSignalSyncKHR (glxw_egl->_eglSignalSyncKHR)
#define eglGetSyncAttribKHR (glxw_egl->_eglGetSyncAttribKHR)
#define eglCreateFenceSyncNV (glxw_egl->_eglCreateFenceSyncNV)
#define eglDestroySyncNV (glxw_egl->_eglDestroySyncNV)
#define eglFenceNV (glxw_egl->_eglFenceNV)
#define eglClientWaitSyncNV (glxw_egl->_eglClientWaitSyncNV)
#define eglSignalSyncNV (glxw_egl->_eglSignalSyncNV)
#define eglGetSyncAttribNV (glxw_egl->_eglGetSyncAttribNV)
#define eglCreatePixmapSurfaceHI (glxw_egl->_eglCreatePixmapSurfaceHI)
#define eglCreateDRMImageMESA (glxw_egl->_eglCreateDRMImageMESA)
#define eglExportDRMImageMESA (glxw_egl->_eglExportDRMImageMESA)
#define eglPostSubBufferNV (glxw_egl->_eglPostSubBufferNV)
#define eglGetSystemTimeFrequencyNV (glxw_egl->_eglGetSystemTimeFrequencyNV)
#define eglGetSystemTimeNV (glxw_egl->_eglGetSystemTimeNV)
#define eglCreateStreamKHR (glxw_egl->_eglCreateStreamKHR)
#define eglDestroyStreamKHR (glxw_egl->_eglDestroyStreamKHR)
#define eglStreamAttribKHR (glxw_egl->_eglStreamAttribKHR)
#define eglQueryStreamKHR (glxw_egl->_eglQueryStreamKHR)
#define eglQueryStreamu64KHR (glxw_egl->_eglQueryStreamu64KHR)
#define eglStreamConsumerGLTextureExternalKHR (glxw_egl->_eglStreamConsumerGLTextureExternalKHR)
#define eglStreamConsumerAcquireKHR (glxw_egl->_eglStreamConsumerAcquireKHR)
#define eglStreamConsumerReleaseKHR (glxw_egl->_eglStreamConsumerReleaseKHR)
#define eglCreateStreamProducerSurfaceKHR (glxw_egl->_eglCreateStreamProducerSurfaceKHR)
#define eglQueryStreamTimeKHR (glxw_egl->_eglQueryStreamTimeKHR)
#define eglGetStreamFileDescriptorKHR (glxw_egl->_eglGetStreamFileDescriptorKHR)
#define eglCreateStreamFromFileDescriptorKHR (glxw_egl->_eglCreateStreamFromFileDescriptorKHR)
#define eglWaitSyncKHR (glxw_egl->_eglWaitSyncKHR)
#define eglQueryNativeDisplayNV (glxw_egl->_eglQueryNativeDisplayNV)
#define eglQueryNativeWindowNV (glxw_egl->_eglQueryNativeWindowNV)
#define eglQueryNativePixmapNV (glxw_egl->_eglQueryNativePixmapNV)
#define eglSetBlobCacheFuncsANDROID (glxw_egl->_eglSetBlobCacheFuncsANDROID)
#define eglDupNativeFenceFDANDROID (glxw_egl->_eglDupNativeFenceFDANDROID)

#ifdef __cplusplus
}
#endif

#endif
