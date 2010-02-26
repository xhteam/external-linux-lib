/*
 * Copyright 2004-2010 Freescale Semiconductor, Inc.
 *
 * Copyright (c) 2006, Chips & Media.  All rights reserved.
 */

/*
 * The code contained herein is licensed under the GNU Lesser General
 * Public License.  You may obtain a copy of the GNU Lesser General
 * Public License Version 2.1 or later at the following locations:
 *
 * http://www.opensource.org/licenses/lgpl-license.html
 * http://www.gnu.org/copyleft/lgpl.html
 */

#ifndef _VPU_UTIL_H_
#define _VPU_UTIL_H_

#include <sys/types.h>
#include <pthread.h>
#include <errno.h>

#include "vpu_reg.h"
#include "vpu_lib.h"
#include "vpu_io.h"

#define MAX_FW_BINARY_LEN		100 * 1024
#define MAX_NUM_INSTANCE		4

#define SIZE_PIC_PARA_BASE_BUF          0x100
#define SIZE_MV_DATA                    0x20000
#define SIZE_MB_DATA                    0x4000
#define SIZE_FRAME_BUF_STAT             0x100
#define SIZE_SLICE_INFO                 0x4000
#define USER_DATA_INFO_OFFSET           8*17

#define ADDR_PIC_PARA_BASE_OFFSET       0
#define ADDR_MV_BASE_OFFSET             ADDR_PIC_PARA_BASE_OFFSET + SIZE_PIC_PARA_BASE_BUF
#define ADDR_MB_BASE_OFFSET             ADDR_MV_BASE_OFFSET + SIZE_MV_DATA
#define ADDR_FRAME_BUF_STAT_BASE_OFFSET ADDR_MB_BASE_OFFSET + SIZE_MB_DATA
#define ADDR_SLICE_BASE_OFFSET          ADDR_MB_BASE_OFFSET + SIZE_MB_DATA
#define ENC_ADDR_END_OF_RPT_BUF         ADDR_FRAME_BUF_STAT_BASE_OFFSET + SIZE_SLICE_INFO
#define DEC_ADDR_END_OF_RPT_BUF         ADDR_FRAME_BUF_STAT_BASE_OFFSET + SIZE_FRAME_BUF_STAT

/* This is calculated by reserved iram size for vpu and requirement */
#if defined(IMX51) || defined(IMX53)
/* 720P for decoder, D1 for encoder on i.MX51 */
#define DEC_MAX_WIDTH_IRAM_SUPPORT      1280
#define ENC_MAX_WIDTH_IRAM_SUPPORT      720
#else
#define DEC_MAX_WIDTH_IRAM_SUPPORT      1920
#define ENC_MAX_WIDTH_IRAM_SUPPORT      1280
#endif

/* Following IRAM setting is for max size for Decoder */
/* To save intermediate data such as MB prediction data */
#define VPU_DEC_BIT_IRAM_OFFSET         0
#define VPU_DEC_BIT_IRAM_SIZE           ((128 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* To save intermediate data such as intra/ACDC prediction */
#define VPU_DEC_IP_IRAM_OFFSET          (VPU_DEC_BIT_IRAM_OFFSET + VPU_DEC_BIT_IRAM_SIZE)
#define VPU_DEC_IP_IRAM_SIZE            ((128 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* To save intermediate data for deblocking filter */
#define VPU_DEC_DBKY_IRAM_OFFSET        (VPU_DEC_IP_IRAM_OFFSET + VPU_DEC_IP_IRAM_SIZE)
#define VPU_DEC_DBKY_IRAM_SIZE          ((256 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
#define VPU_DEC_DBKC_IRAM_OFFSET        (VPU_DEC_DBKY_IRAM_OFFSET + VPU_DEC_DBKY_IRAM_SIZE)
#define VPU_DEC_DBKC_IRAM_SIZE          ((256 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* To save intermediate data for overlap filter */
#define VPU_DEC_OVL_IRAM_OFFSET         (VPU_DEC_DBKC_IRAM_OFFSET + VPU_DEC_DBKC_IRAM_SIZE)
#define VPU_DEC_OVL_IRAM_SIZE           ((80 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
#define VPU_DEC_TOTAL_IRAM_SIZE         (VPU_DEC_OVL_IRAM_OFFSET + VPU_DEC_OVL_IRAM_SIZE)

/* Following IRAM setting is for max size for Encoder */
#define VPU_ENC_SEARCH_IRAM_OFFSET      0
#define VPU_ENC_SEARCH_IRAM_SIZE        ((ENC_MAX_WIDTH_IRAM_SUPPORT * 36 + 2048 + 1023) & ~1023)
/* To save intermediate data such as MB prediction data */
#define VPU_ENC_BIT_IRAM_OFFSET         (VPU_ENC_SEARCH_IRAM_OFFSET + VPU_ENC_SEARCH_IRAM_SIZE)
#define VPU_ENC_BIT_IRAM_SIZE           ((128 * ENC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* To save intermediate data such as intra/ACDC prediction */
#define VPU_ENC_IP_IRAM_OFFSET          (VPU_ENC_BIT_IRAM_OFFSET + VPU_ENC_BIT_IRAM_SIZE)
#define VPU_ENC_IP_IRAM_SIZE            ((128 * ENC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* To save intermediate data for deblocking filter */
/* Max size is calculated by H264/H263 encoder */
#define VPU_ENC_DBKY_IRAM_OFFSET        (VPU_ENC_IP_IRAM_OFFSET + VPU_ENC_IP_IRAM_SIZE)
#define VPU_ENC_DBKY_IRAM_SIZE          ((128 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
#define VPU_ENC_DBKC_IRAM_OFFSET        (VPU_ENC_DBKY_IRAM_OFFSET + VPU_ENC_DBKY_IRAM_SIZE)
#define VPU_ENC_DBKC_IRAM_SIZE          ((128 * DEC_MAX_WIDTH_IRAM_SUPPORT / 16 + 1023) & ~1023)
/* No need for overlap filter for encoder */
#define VPU_ENC_OVL_IRAM_OFFSET         (VPU_ENC_DBKC_IRAM_OFFSET + VPU_ENC_DBKC_IRAM_SIZE)
#define VPU_ENC_OVL_IRAM_SIZE           0
#define VPU_ENC_TOTAL_IRAM_SIZE         (VPU_ENC_OVL_IRAM_OFFSET + VPU_ENC_OVL_IRAM_SIZE)

#if defined(IMX37_3STACK)
enum {
	AVC_DEC = 0,
	VC1_DEC = 1,
	MP2_DEC = 2,
	MP4_DEC = 3,
	DV3_DEC = 4,
	/* dummy */
	AVC_ENC = 0x7F,
	MP4_ENC = 0x80,
	RV_DEC = 0x81,
	MJPG_DEC = 0x82,
	MJPG_ENC = 0x83
};
#elif defined(IMX51) || defined(IMX53)
enum {
	AVC_DEC = 0,
	VC1_DEC = 1,
	MP2_DEC = 2,
	MP4_DEC = 3,
	DV3_DEC = 3,
	RV_DEC = 4,
	MJPG_DEC = 5,
	AVC_ENC = 8,
	MP4_ENC = 11,
	MJPG_ENC = 13
} ;
#else
enum {
	MP4_DEC = 0,
	MP4_ENC = 1,
	AVC_DEC = 2,
	AVC_ENC = 3,
	VC1_DEC = 4,
	MP2_DEC = 5,
	DV3_DEC = 6,
	/* dummy */
	RV_DEC = 0x81,
	MJPG_DEC = 0x82,
	MJPG_ENC = 0x83
};
#endif

enum {
	SEQ_INIT = 1,
	SEQ_END = 2,
	PIC_RUN = 3,
	SET_FRAME_BUF = 4,
	ENCODE_HEADER = 5,
	ENC_PARA_SET = 6,
	DEC_PARA_SET = 7,
	DEC_BUF_FLUSH = 8,
	RC_CHANGE_PARAMETER = 9,
	VPU_WAKE = 11,
	FIRMWARE_GET = 0xf
};

typedef struct {
	int useBitEnable;
	int useIpEnable;
	int useDbkEnable;
	int useOvlEnable;
	int useMeEnable;

	int useHostBitEnable;
	int useHostIpEnable;
	int useHostDbkEnable;
	int useHostOvlEnable;
	int useHostMeEnable;

	PhysicalAddress bufBitUse;
	PhysicalAddress bufIpAcDcUse;
	PhysicalAddress bufDbkYUse;
	PhysicalAddress bufDbkCUse;
	PhysicalAddress bufOvlUse;

	PhysicalAddress searchRamAddr;
	int searchRamSize;

} SecAxiUse;

typedef struct {
	EncOpenParam openParam;
	EncInitialInfo initialInfo;

	PhysicalAddress streamRdPtr;
	PhysicalAddress streamRdPtrRegAddr;
	PhysicalAddress streamWrPtrRegAddr;
	PhysicalAddress streamBufStartAddr;
	PhysicalAddress streamBufEndAddr;
	int streamBufSize;

	FrameBuffer *frameBufPool;
	int numFrameBuffers;
	int stride;

	int rotationEnable;
	int mirrorEnable;
	MirrorDirection mirrorDirection;
	int rotationAngle;

	int initialInfoObtained;
	int dynamicAllocEnable;
	int ringBufferEnable;

	SecAxiUse secAxiUse;

	EncReportInfo encReportMBInfo;
	EncReportInfo encReportMVInfo;
	EncReportInfo encReportSliceInfo;

	vpu_mem_desc picParaBaseMem;

} EncInfo;

typedef struct {
	DecOpenParam openParam;
	DecInitialInfo initialInfo;

	PhysicalAddress streamWrPtr;
	PhysicalAddress streamRdPtrRegAddr;
	PhysicalAddress streamWrPtrRegAddr;
	PhysicalAddress streamBufStartAddr;
	PhysicalAddress streamBufEndAddr;
	PhysicalAddress frameDisplayFlagRegAddr;
	int streamBufSize;

	FrameBuffer *frameBufPool;
	int numFrameBuffers;
	FrameBuffer *recFrame;
	int stride;

	int rotationEnable;
	int deringEnable;
	int mirrorEnable;
	MirrorDirection mirrorDirection;
	int rotationAngle;
	FrameBuffer rotatorOutput;
	int rotatorStride;
	int rotatorOutputValid;
	int initialInfoObtained;

	FrameBuffer deBlockingFilterOutput;
	int deBlockingFilterOutputValid;

	int filePlayEnable;
	int picSrcSize;
	int dynamicAllocEnable;
	int vc1BframeDisplayValid;

	DbkOffset dbkOffset;

	SecAxiUse secAxiUse;

	vpu_mem_desc picParaBaseMem;
	vpu_mem_desc userDataBufMem;

	DecReportInfo decReportFrameBufStat; /* Frame Buffer Status */
	DecReportInfo decReportMBInfo;      /* Mb Param for Error Concealment */
	DecReportInfo decReportMVInfo;     /* Motion vector */
	DecReportInfo decReportUserData;
} DecInfo;

typedef struct CodecInst {
	int instIndex;
	int inUse;
	int codecMode;
	union {
		EncInfo encInfo;
		DecInfo decInfo;
	} CodecInfo;
	union {
		EncParam encParam;
		DecParam decParam;
	} CodecParam;
} CodecInst;

typedef struct {
	int is_initialized;
	pthread_mutex_t lock;

	/* VPU data for sharing */
	CodecInst codecInstPool[MAX_NUM_INSTANCE];
	CodecInst *pendingInst;
} semaphore_t;

void BitIssueCommand(int instIdx, int cdcMode, int cmd);

RetCode LoadBitCodeTable(Uint16 * pBitCode, int *size);
RetCode DownloadBitCodeTable(unsigned long *virtCodeBuf, Uint16 *bit_code);

RetCode GetCodecInstance(CodecInst ** ppInst);
void FreeCodecInstance(CodecInst * pCodecInst);

RetCode CheckInstanceValidity(CodecInst * pci);
RetCode CheckEncInstanceValidity(EncHandle handle);
RetCode CheckEncOpenParam(EncOpenParam * pop);
RetCode CheckEncParam(CodecInst * pCodecInst, EncParam * param);
void EncodeHeader(EncHandle handle, EncHeaderParam * encHeaderParam);
void GetParaSet(EncHandle handle, int paraSetType, EncParamSet * para);

RetCode CheckDecInstanceValidity(DecHandle handle);
RetCode CheckDecOpenParam(DecOpenParam * pop);
int DecBitstreamBufEmpty(DecInfo * pDecInfo);
void SetParaSet(DecHandle handle, int paraSetType, DecParamSet * para);
RetCode CopyBufferData(Uint8 *dst, Uint8 *src, int size);

RetCode SetGopNumber(EncHandle handle, Uint32 *gopNumber);
RetCode SetIntraQp(EncHandle handle, Uint32 *intraQp);
RetCode SetBitrate(EncHandle handle, Uint32 *bitrate);
RetCode SetFramerate(EncHandle handle, Uint32 *framerate);
RetCode SetIntraRefreshNum(EncHandle handle, Uint32 *pIntraRefreshNum);
RetCode SetSliceMode(EncHandle handle, EncSliceMode *pSliceMode);
RetCode SetHecMode(EncHandle handle, int mode);

void SetDecSecondAXIIRAM(SecAxiUse *psecAxiIramInfo, int width);
void SetEncSecondAXIIRAM(SecAxiUse *psecAxiIramInfo, int width);

semaphore_t *vpu_semaphore_open(void);
void semaphore_post(semaphore_t *semap);
bool semaphore_wait(semaphore_t *semap);
void vpu_semaphore_close(semaphore_t *semap);

static inline bool LockVpu(semaphore_t *semap)
{
	if (!semaphore_wait(semap))
		return false;
	IOClkGateSet(1);
	return true;
}

static inline void UnlockVpu(semaphore_t *semap)
{
	semaphore_post(semap);
	IOClkGateSet(0);
}

#define swab32(x) \
	((Uint32)( \
		(((Uint32)(x) & (Uint32)0x000000ffUL) << 24) | \
		(((Uint32)(x) & (Uint32)0x0000ff00UL) <<  8) | \
		(((Uint32)(x) & (Uint32)0x00ff0000UL) >>  8) | \
		(((Uint32)(x) & (Uint32)0xff000000UL) >> 24) ))

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

#endif
