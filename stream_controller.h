#ifndef __STREAM_CONTROLLER_H__
#define __STREAM_CONTROLLER_H__

#include <stdio.h>
#include "tables.h"
#include "tdp_api.h"
#include "tables.h"
#include "pthread.h"
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#define DESIRED_FREQUENCY 754000000	        /* Tune frequency in Hz */
#define BANDWIDTH 8    				        /* Bandwidth in Mhz */
#define LINELEN 50

/**
 * @brief Structure that defines stream controller error
 */
typedef enum _StreamControllerError
{
    SC_NO_ERROR = 0,
    SC_ERROR,
    SC_THREAD_ERROR
}StreamControllerError;

/**
 * @brief Structure that defines channel info
 */
typedef struct _ChannelInfo
{
    int16_t programNumber;
    int16_t audioPid;
    int16_t videoPid;
}ChannelInfo;


typedef struct _ConfigFileInfo{
    int16_t Frequency;
    int16_t Bandwidth;
    int16_t progNumber;
    t_Module Modul;
}ConfigFileInfo;

/**
 * @brief Initializes stream controller module
 *
 * @return stream controller error code
 */
StreamControllerError streamControllerInit();

/**
 * @brief Deinitializes stream controller module
 *
 * @return stream controller error code
 */
StreamControllerError streamControllerDeinit();

/**
 * @brief Channel up
 *
 * @return stream controller error
 */
StreamControllerError channelUp();

/**
 * @brief Channel down
 *
 * @return stream controller error
 */
StreamControllerError channelDown();

/**
 * @brief Returns current channel info
 *
 * @param [out] channelInfo - channel info structure with current channel info
 * @return stream controller error code
 */
StreamControllerError getChannelInfo(ChannelInfo* channelInfo);

/*
 * @brief Load config file informations for some parameters
 * @param [in] filename-name of file to be loaded
 * @return stream contorller error code
*/
StreamControllerError getConfigFile(char* filename);

#endif /* __STREAM_CONTROLLER_H__ */
