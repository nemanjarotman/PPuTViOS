#ifndef __TABLES_H__
#define __TABLES_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define TABLES_MAX_NUMBER_OF_PIDS_IN_PAT    20 	    /* Max number of PMT pids in one PAT table */
#define TABLES_MAX_NUMBER_OF_ELEMENTARY_PID 20       /* Max number of elementary pids in one PMT table */
#define TABLES_MAX_NUMBER_OF_LOCAL_TIMES_DSC 20
#define TABLES_MAX_NUMBER_OF_TOT_DSC
/**
 * @brief Enumeration of possible tables parser error codes
 */
typedef enum _ParseErrorCode
{
    TABLES_PARSE_ERROR = 0,                         /* TABLES_PARSE_ERROR */
	TABLES_PARSE_OK = 1                             /* TABLES_PARSE_OK */
}ParseErrorCode;

/**
 * @brief Structure that defines PAT Table Header
 */
typedef struct _PatHeader
{
    uint8_t     tableId;                            /* The type of table */
    uint8_t     sectionSyntaxIndicator;             /* The format of the table section to follow */
    uint16_t    sectionLength;                      /* The length of the table section beyond this field */
    uint16_t    transportStreamId;                  /* Transport stream identifier */
    uint8_t     versionNumber;                      /* The version number the private table section */
    uint8_t     currentNextIndicator;               /* Signals what a particular table will look like when it next changes */
    uint8_t     sectionNumber;                      /* Section number */
    uint8_t     lastSectionNumber;                  /* Signals the last section that is valid for a particular MPEG-2 private table */
}PatHeader;

/**
 * @brief Structure that defines PAT service info
 */
typedef struct _PatServiceInfo
{
    uint16_t    programNumber;                      /* Identifies each service present in a transport stream */
    uint16_t    pid;                                /* Pid of Program Map table section or pid of Network Information Table  */
}PatServiceInfo;

/**
 * @brief Structure that defines PAT table
 */
typedef struct _PatTable
{
    PatHeader patHeader;                                                     /* PAT Table Header */
    PatServiceInfo patServiceInfoArray[TABLES_MAX_NUMBER_OF_PIDS_IN_PAT];    /* Services info presented in PAT table */
    uint8_t serviceInfoCount;                                                /* Number of services info presented in PAT table */
}PatTable;

/**
 * @brief Structure that defines PMT table header
 */
typedef struct _PmtTableHeader
{
    uint8_t tableId;
    uint8_t sectionSyntaxIndicator;
    uint16_t sectionLength;
    uint16_t programNumber;
    uint8_t versionNumber;
    uint8_t currentNextIndicator;
    uint8_t sectionNumber;
    uint8_t lastSectionNumber;
    uint16_t pcrPid;
    uint16_t programInfoLength;
}PmtTableHeader;

/**
 * @brief Structure that defines PMT elementary info
 */
typedef struct _PmtElementaryInfo
{
    uint8_t streamType;
    uint16_t elementaryPid;
    uint16_t esInfoLength;
}PmtElementaryInfo;

/**
 * @brief Structure that defines PMT table
 */
typedef struct _PmtTable
{
    PmtTableHeader pmtHeader;
    PmtElementaryInfo pmtElementaryInfoArray[TABLES_MAX_NUMBER_OF_ELEMENTARY_PID];
    uint8_t elementaryInfoCount;
}PmtTable;


typedef struct _TdtTable{
	uint8_t table_id;
	uint8_t section_syntax_indicator;
	uint16_t section_length;
	uint64_t mjd;
	uint8_t month;
	uint8_t day;
	uint8_t year;
}TdtTable

typedef struct _TotTable{
	uint8_t table_id;
	uint8_t section_syntax_indicator;
	uint16_t section_length;
	uint16_t mjd;
	uint16_t descriptors_loop_length;
}TotTable;

typedef struct _LocalTimeDescriptor{
    uint8_t ch1;
    uint8_t ch2;
    uint8_t ch3;
    uint8_t region;
    uint8_t ltPolarity;
    uint8_t ltHours;
    uint8_t ltMin;
}localTimeDescriptor



/**
 * @brief  Parse PAT header.
 *
 * @param  [in]   patHeaderBuffer Buffer that contains PAT header
 * @param  [out]  patHeader PAT header
 * @return tables error code
 */
ParseErrorCode parsePatHeader(const uint8_t* patHeaderBuffer, PatHeader* patHeader);

/**
 * @brief  Parse PAT Service information.
 *
 * @param  [in]   patServiceInfoBuffer Buffer that contains PAT Service info
 * @param  [out]  descriptor PAT Service info
 * @return tables error code
 */
ParseErrorCode parsePatServiceInfo(const uint8_t* patServiceInfoBuffer, PatServiceInfo* patServiceInfo);

/**
 * @brief  Parse PAT Table.
 *
 * @param  [in]   patSectionBuffer Buffer that contains PAT table section
 * @param  [out]  patTable PAT Table
 * @return tables error code
 */
ParseErrorCode parsePatTable(const uint8_t* patSectionBuffer, PatTable* patTable);

/**
 * @brief  Print PAT Table
 *
 * @param  [in]   patTable PAT table to be printed
 * @return tables error code
 */
ParseErrorCode printPatTable(PatTable* patTable);

/**
 * @brief Parse pmt table
 *
 * @param [in]  pmtHeaderBuffer Buffer that contains PMT header
 * @param [out] pmtHeader PMT table header
 * @return tables error code
 */
ParseErrorCode parsePmtHeader(const uint8_t* pmtHeaderBuffer, PmtTableHeader* pmtHeader);

/**
 * @brief Parse PMT elementary info
 *
 * @param [in]  pmtElementaryInfoBuffer Buffer that contains pmt elementary info
 * @param [out] PMT elementary info
 * @return tables error code
 */
ParseErrorCode parsePmtElementaryInfo(const uint8_t* pmtElementaryInfoBuffer, PmtElementaryInfo* pmtElementaryInfo);

/**
 * @brief Parse PMT table
 *
 * @param [in]  pmtSectionBuffer Buffer that contains pmt table section
 * @param [out] pmtTable PMT table
 * @return tables error code
 */
ParseErrorCode parsePmtTable(const uint8_t* pmtSectionBuffer, PmtTable* pmtTable);

/**
 * @brief Print PMT table
 *
 * @param [in] pmtTable PMT table
 * @return tables error code
 */
ParseErrorCode printPmtTable(PmtTable* pmtTable);

ParseErrorCode parseTdtTable(const uint8_t* tdtSectionBuffer,TdtTable* tdtTable);

ParseErrorCode printTdtTable(TdtTable* tdtTable);

ParseErrorCode parseTotTable(const uint8_t* totSectionBuffer,TotTable* totTable);

ParseErrorCode printTotTable(TotTable* TotTable);

#endif /* __TABLES_H__ */
