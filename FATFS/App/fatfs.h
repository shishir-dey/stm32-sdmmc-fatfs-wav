/**
 ******************************************************************************
 * @file   fatfs.h
 * @brief  Header for fatfs applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __fatfs_H
#define __fatfs_H
#ifdef __cplusplus
extern "C"
{
#endif

#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h" /* defines SD_Driver as external */

/* USER CODE BEGIN Includes */
/* typedefs and defines */
#define FILEMGR_LIST_DEPDTH 24
#define FILEMGR_FILE_NAME_SIZE 40
#define FILEMGR_FULL_PATH_SIZE 256
#define FILEMGR_MAX_LEVEL 4
#define FILETYPE_DIR 0
#define FILETYPE_FILE 1

typedef struct {
	uint32_t ChunkID; /* 0 */
	uint32_t FileSize; /* 4 */
	uint32_t FileFormat; /* 8 */
	uint32_t SubChunk1ID; /* 12 */
	uint32_t SubChunk1Size; /* 16*/
	uint16_t AudioFormat; /* 20 */
	uint16_t NbrChannels; /* 22 */
	uint32_t SampleRate; /* 24 */

	uint32_t ByteRate; /* 28 */
	uint16_t BlockAlign; /* 32 */
	uint16_t BitPerSample; /* 34 */
	uint32_t SubChunk2ID; /* 36 */
	uint32_t SubChunk2Size; /* 40 */
} WAVE_FormatTypeDef;

typedef enum {
	WAV_OK, WAV_ERROR
} WAV_StatusTypeDef;

typedef struct _FILELIST_LineTypeDef {
	uint8_t type;
	uint8_t name[FILEMGR_FILE_NAME_SIZE];
} FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef {
	FILELIST_LineTypeDef file[FILEMGR_LIST_DEPDTH];
	uint16_t ptr;
} FILELIST_FileTypeDef;
/* USER CODE END Includes */

extern uint8_t retSD; /* Return value for SD */
extern char SDPath[4]; /* SD logical drive path */
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */

void MX_FATFS_Init(void);

/* USER CODE BEGIN Prototypes */
FRESULT WAV_StorageParse(void);
uint16_t WAV_GetFDCurrentPos(void);
uint8_t WAV_GetFileDescriptor(const char *fileName);
WAV_StatusTypeDef WAV_GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info);
/* USER CODE END Prototypes */
#ifdef __cplusplus
}
#endif
#endif /*__fatfs_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
