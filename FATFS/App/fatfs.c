/**
 ******************************************************************************
 * @file   fatfs.c
 * @brief  Code for fatfs applications
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

#include <string.h>
#include "fatfs.h"

uint8_t retSD; /* Return value for SD */
char SDPath[4]; /* SD logical drive path */
FATFS SDFatFS; /* File system object for SD logical drive */
FIL SDFile; /* File object for SD */

/* USER CODE BEGIN Variables */
extern SD_HandleTypeDef hsd1;
FIL WavFile;
/* USER CODE END Variables */

void MX_FATFS_Init(void) {
	/*## FatFS: Link the SD driver ###########################*/
	retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

	/* USER CODE BEGIN Init */
	/* additional user code for init */
	/* USER CODE END Init */
}

/**
 * @brief  Gets Time from RTC
 * @param  None
 * @retval Time in DWORD
 */
DWORD get_fattime(void) {
	/* USER CODE BEGIN get_fattime */
	return 0;
	/* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
FILELIST_FileTypeDef FileList;
uint16_t fd = 0; /* File descriptor */

/**
 * @brief  Detect all WAV files in the file system, and add them to FileList
 * @param  None
 * @retval Operation result
 */
FRESULT WAV_StorageParse(void) {
	FRESULT res = FR_OK;
	FILINFO fno;
	DIR dir;
	char *fn;

	res = f_mount(&SDFatFS, SDPath, 1);

	if (res != FR_OK) {
		return res;
	}

	res = f_opendir(&dir, SDPath);
	FileList.ptr = 0;

	if (res == FR_OK) {
		while (HAL_SD_GetState(&hsd1) == HAL_SD_STATE_READY) {
			res = f_readdir(&dir, &fno);
			if (res != FR_OK || fno.fname[0] == 0) {
				break;
			}
			if (fno.fname[0] == '.') {
				continue;
			}
			fn = fno.fname;

			if (FileList.ptr < FILEMGR_LIST_DEPDTH) {
				if ((fno.fattrib & AM_DIR) == 0) {
					if ((strstr(fn, "wav")) || (strstr(fn, "WAV"))) {
						strncpy((char*) FileList.file[FileList.ptr].name,
								(char*) fn, FILEMGR_FILE_NAME_SIZE);
						FileList.file[FileList.ptr].type = FILETYPE_FILE;
						FileList.ptr++;
					}
				}
			}
		}
	}
	fd = FileList.ptr;
	f_closedir(&dir);
	return res;
}

/**
 * @brief  Gets WAV file count
 * @param  None
 * @retval None
 */
uint16_t WAV_GetFDCurrentPos(void) {
	return fd;
}

/**
 * @brief  Gets WAV file descriptor
 * @param  None
 * @retval None
 */
uint8_t WAV_GetFileDescriptor(const char *fileName) {
	uint8_t i;
	for (i = 0; i < 10; i++) {
		if (strcmp((char*) FileList.file[i].name, fileName) == 0) {
			return i;
		}
	}
	return -1;
}

/**
 * @brief  Gets the file info.
 * @param  file_idx: File index
 * @param  info: Pointer to WAV file info
 * @retval Status of the operation
 */
WAV_StatusTypeDef WAV_GetFileInfo(uint16_t file_idx, WAVE_FormatTypeDef *info) {
	uint32_t bytesread;

	if (f_open(&WavFile, (char*) FileList.file[file_idx].name,
			FA_OPEN_EXISTING | FA_READ) == FR_OK) {
		if (f_read(&WavFile, info, sizeof(WAVE_FormatTypeDef), (void*) &bytesread)
				== FR_OK) {
			// uint32_t duration = info->FileSize / info->ByteRate;
			return WAV_ERROR;
		}
		f_close(&WavFile);
	}
	return WAV_OK;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
