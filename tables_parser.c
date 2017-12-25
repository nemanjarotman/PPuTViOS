#include "tables.h"


ParseErrorCode parsePatHeader(const uint8_t* patHeaderBuffer, PatHeader* patHeader)
{
    if(patHeaderBuffer==NULL || patHeader==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    patHeader->tableId = (uint8_t)* patHeaderBuffer;
    if (patHeader->tableId != 0x00)
    {
        printf("\n%s : ERROR it is not a PAT Table\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    uint8_t lower8Bits = 0;
    uint8_t higher8Bits = 0;
    uint16_t all16Bits = 0;

    lower8Bits = (uint8_t)(*(patHeaderBuffer + 1));
    lower8Bits = lower8Bits >> 7;
    patHeader->sectionSyntaxIndicator = lower8Bits & 0x01;

    higher8Bits = (uint8_t) (*(patHeaderBuffer + 1));
    lower8Bits = (uint8_t) (*(patHeaderBuffer + 2));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    patHeader->sectionLength = all16Bits & 0x0FFF;

    higher8Bits = (uint8_t) (*(patHeaderBuffer + 3));
    lower8Bits = (uint8_t) (*(patHeaderBuffer + 4));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    patHeader->transportStreamId = all16Bits & 0xFFFF;

    lower8Bits = (uint8_t) (*(patHeaderBuffer + 5));
    lower8Bits = lower8Bits >> 1;
    patHeader->versionNumber = lower8Bits & 0x1F;

    lower8Bits = (uint8_t) (*(patHeaderBuffer + 5));
    patHeader->currentNextIndicator = lower8Bits & 0x01;

    lower8Bits = (uint8_t) (*(patHeaderBuffer + 6));
    patHeader->sectionNumber = lower8Bits & 0xFF;

    lower8Bits = (uint8_t) (*(patHeaderBuffer + 7));
    patHeader->lastSectionNumber = lower8Bits & 0xFF;

    return TABLES_PARSE_OK;
}

ParseErrorCode parsePatServiceInfo(const uint8_t* patServiceInfoBuffer, PatServiceInfo* patServiceInfo)
{
    if(patServiceInfoBuffer==NULL || patServiceInfo==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    uint8_t lower8Bits = 0;
    uint8_t higher8Bits = 0;
    uint16_t all16Bits = 0;

    higher8Bits = (uint8_t) (*(patServiceInfoBuffer));
    lower8Bits = (uint8_t) (*(patServiceInfoBuffer + 1));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    patServiceInfo->programNumber = all16Bits & 0xFFFF;

    higher8Bits = (uint8_t) (*(patServiceInfoBuffer + 2));
    lower8Bits = (uint8_t) (*(patServiceInfoBuffer + 3));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    patServiceInfo->pid = all16Bits & 0x1FFF;

    return TABLES_PARSE_OK;
}

ParseErrorCode parsePatTable(const uint8_t* patSectionBuffer, PatTable* patTable)
{
    uint8_t * currentBufferPosition = NULL;
    uint32_t parsedLength = 0;

    if(patSectionBuffer==NULL || patTable==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    if(parsePatHeader(patSectionBuffer,&(patTable->patHeader))!=TABLES_PARSE_OK)
    {
        printf("\n%s : ERROR parsing PAT header\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    parsedLength = 12 /*PAT header size*/ - 3 /*Not in section length*/;
    currentBufferPosition = (uint8_t *)(patSectionBuffer + 8); /* Position after last_section_number */
    patTable->serviceInfoCount = 0; /* Number of services info presented in PAT table */

    while(parsedLength < patTable->patHeader.sectionLength)
    {
        if(patTable->serviceInfoCount > TABLES_MAX_NUMBER_OF_PIDS_IN_PAT - 1)
        {
            printf("\n%s : ERROR there is not enough space in PAT structure for Service info\n", __FUNCTION__);
            return TABLES_PARSE_ERROR;
        }

        if(parsePatServiceInfo(currentBufferPosition, &(patTable->patServiceInfoArray[patTable->serviceInfoCount])) == TABLES_PARSE_OK)
        {
            currentBufferPosition += 4; /* Size from program_number to pid */
            parsedLength += 4; /* Size from program_number to pid */
            patTable->serviceInfoCount ++;
        }
    }

    return TABLES_PARSE_OK;
}

ParseErrorCode printPatTable(PatTable* patTable)
{
    uint8_t i=0;

    if(patTable==NULL)
    {
        printf("\n%s : ERROR received parameter is not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    printf("\n********************PAT TABLE SECTION********************\n");
    printf("table_id                 |      %d\n",patTable->patHeader.tableId);
    printf("section_length           |      %d\n",patTable->patHeader.sectionLength);
    printf("transport_stream_id      |      %d\n",patTable->patHeader.transportStreamId);
    printf("section_number           |      %d\n",patTable->patHeader.sectionNumber);
    printf("last_section_number      |      %d\n",patTable->patHeader.lastSectionNumber);

    for (i=0; i<patTable->serviceInfoCount;i++)
    {
        printf("-----------------------------------------\n");
        printf("program_number           |      %d\n",patTable->patServiceInfoArray[i].programNumber);
        printf("pid                      |      %d\n",patTable->patServiceInfoArray[i].pid);
    }
    printf("\n********************PAT TABLE SECTION********************\n");

    return TABLES_PARSE_OK;
}


ParseErrorCode parsePmtHeader(const uint8_t* pmtHeaderBuffer, PmtTableHeader* pmtHeader)
{

    if(pmtHeaderBuffer==NULL || pmtHeader==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    pmtHeader->tableId = (uint8_t)* pmtHeaderBuffer;
    if (pmtHeader->tableId != 0x02)
    {
        printf("\n%s : ERROR it is not a PMT Table\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    uint8_t lower8Bits = 0;
    uint8_t higher8Bits = 0;
    uint16_t all16Bits = 0;

    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 1));
    lower8Bits = lower8Bits >> 7;
    pmtHeader->sectionSyntaxIndicator = lower8Bits & 0x01;

    higher8Bits = (uint8_t) (*(pmtHeaderBuffer + 1));
    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 2));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtHeader->sectionLength = all16Bits & 0x0FFF;

    higher8Bits = (uint8_t) (*(pmtHeaderBuffer + 3));
    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 4));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtHeader->programNumber = all16Bits & 0xFFFF;

    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 5));
    lower8Bits = lower8Bits >> 1;
    pmtHeader->versionNumber = lower8Bits & 0x1F;

    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 5));
    pmtHeader->currentNextIndicator = lower8Bits & 0x01;

    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 6));
    pmtHeader->sectionNumber = lower8Bits & 0xFF;

    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 7));
    pmtHeader->lastSectionNumber = lower8Bits & 0xFF;

    higher8Bits = (uint8_t) (*(pmtHeaderBuffer + 8));
    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 9));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtHeader->pcrPid = all16Bits & 0xFFFF;

    higher8Bits = (uint8_t) (*(pmtHeaderBuffer + 10));
    lower8Bits = (uint8_t) (*(pmtHeaderBuffer + 11));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtHeader->programInfoLength = all16Bits & 0x0FFF;

    return TABLES_PARSE_OK;
}

ParseErrorCode parsePmtElementaryInfo(const uint8_t* pmtElementaryInfoBuffer, PmtElementaryInfo* pmtElementaryInfo)
{
    if(pmtElementaryInfoBuffer==NULL || pmtElementaryInfo==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    uint8_t lower8Bits = 0;
    uint8_t higher8Bits = 0;
    uint16_t all16Bits = 0;

    pmtElementaryInfo->streamType = (uint8_t) (*(pmtElementaryInfoBuffer));

    higher8Bits = (uint8_t) (*(pmtElementaryInfoBuffer + 1));
    lower8Bits = (uint8_t) (*(pmtElementaryInfoBuffer + 2));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtElementaryInfo->elementaryPid = all16Bits & 0x1FFF;

    higher8Bits = (uint8_t) (*(pmtElementaryInfoBuffer + 3));
    lower8Bits = (uint8_t) (*(pmtElementaryInfoBuffer + 4));
    all16Bits = (uint16_t) ((higher8Bits << 8) + lower8Bits);
    pmtElementaryInfo->esInfoLength = all16Bits & 0x0FFF;

    return TABLES_PARSE_OK;
}

ParseErrorCode parsePmtTable(const uint8_t* pmtSectionBuffer, PmtTable* pmtTable)
{
    uint8_t * currentBufferPosition = NULL;
    uint32_t parsedLength = 0;

    if(pmtSectionBuffer==NULL || pmtTable==NULL)
    {
        printf("\n%s : ERROR received parameters are not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    if(parsePmtHeader(pmtSectionBuffer,&(pmtTable->pmtHeader))!=TABLES_PARSE_OK)
    {
        printf("\n%s : ERROR parsing PMT header\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    parsedLength = 12 + pmtTable->pmtHeader.programInfoLength /*PMT header size*/ + 4 /*CRC size*/ - 3 /*Not in section length*/;
    currentBufferPosition = (uint8_t *)(pmtSectionBuffer + 12 + pmtTable->pmtHeader.programInfoLength); /* Position after last descriptor */
    pmtTable->elementaryInfoCount = 0; /* Number of elementary info presented in PMT table */

    while(parsedLength < pmtTable->pmtHeader.sectionLength)
    {
        if(pmtTable->elementaryInfoCount > TABLES_MAX_NUMBER_OF_ELEMENTARY_PID - 1)
        {
            printf("\n%s : ERROR there is not enough space in PMT structure for elementary info\n", __FUNCTION__);
            return TABLES_PARSE_ERROR;
        }

        if(parsePmtElementaryInfo(currentBufferPosition, &(pmtTable->pmtElementaryInfoArray[pmtTable->elementaryInfoCount])) == TABLES_PARSE_OK)
        {
            currentBufferPosition += 5 + pmtTable->pmtElementaryInfoArray[pmtTable->elementaryInfoCount].esInfoLength; /* Size from stream type to elemntary info descriptor*/
            parsedLength += 5 + pmtTable->pmtElementaryInfoArray[pmtTable->elementaryInfoCount].esInfoLength; /* Size from stream type to elementary info descriptor */
            pmtTable->elementaryInfoCount++;
        }
    }

    return TABLES_PARSE_OK;
}

ParseErrorCode printPmtTable(PmtTable* pmtTable)
{
    uint8_t i=0;

    if(pmtTable==NULL)
    {
        printf("\n%s : ERROR received parameter is not ok\n", __FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    printf("\n********************PMT TABLE SECTION********************\n");
    printf("table_id                 |      %d\n",pmtTable->pmtHeader.tableId);
    printf("section_length           |      %d\n",pmtTable->pmtHeader.sectionLength);
    printf("program_number           |      %d\n",pmtTable->pmtHeader.programNumber);
    printf("section_number           |      %d\n",pmtTable->pmtHeader.sectionNumber);
    printf("last_section_number      |      %d\n",pmtTable->pmtHeader.lastSectionNumber);
    printf("program_info_legth       |      %d\n",pmtTable->pmtHeader.programInfoLength);

    for (i=0; i<pmtTable->elementaryInfoCount;i++)
    {
        printf("-----------------------------------------\n");
        printf("stream_type              |      %d\n",pmtTable->pmtElementaryInfoArray[i].streamType);
        printf("elementary_pid           |      %d\n",pmtTable->pmtElementaryInfoArray[i].elementaryPid);
    }
    printf("\n********************PMT TABLE SECTION********************\n");

    return TABLES_PARSE_OK;
}


ParseErrorCode parseTdtTable(const* uint8_t tdtSectionBuffer,TdtTable* tdtTable){
    uint8_t lower8Bits=0;
    uint8_t higher8Bits=0;
    uint16_t all16Bits=0

	  if(tdtSectionBuffer==NULL || tdtTable==NULL){
        printf("\n%s : ERROR received parameters are npot ok\n", __FUNCTION__);
		    return TABLES_PARSE_ERROR;
	  }

    tdtTable->table_id=(uint8_t)* tdtSectionBuffer;

    higher8Bits=(uint8_t) *(tdtSectionBuffer+1);
    lower8Bits=(uint8_t) *(tdtSectionBuffer+2);
    all16Bits=(uint16_t) *((higher8Bits<<8)+lower8Bits);
    tdtTable->all16Bits & 0x0FFF;

    higher8Bits=(uint8_t) *(tdtSectionBuffer+3);
    lower8Bits=(uint8_t) *(tdtSectionBuffer+4);
    all16Bits=(uint8_t) *((higher8Bits<<8))+lower8Bits);
    tdtTable->mjd=all16Bits;

    tdtTable->primY=(int) ((tdtTable->mjd-15078.2)/365.25);
    tdtTable->primM=(int) ((tdtTable->mjd-14956.1-(int) (tdtTable->primY*365.25))/30.6001);

    if(tdtTable->primM==14 || tdtTable->primM==15){
        tdtTable->K=1;
    }else{
        tdtTable->K=0;
    }

    tdtTable->primY=tdtTable->primY+tdtTable->K;
    tdtTable->primM=tdtTable->primM-1 - tdtTable->K*12;
    tdtTable->day=tdtTable->mjd-14987-(int) (tdtTable->primY*365.25)-(int) (tdtTable->primM*30.6001);
    tdtTable->year=tdtTable->primY+1900;
    tdtTable->wday=((tdtTable->mjd+2)%7)+1;

  return TABLES_PARSE_OK;
}

ParseErrorCode printTdtTable(TdtTable tdtTable){

    if(tdtTable==NULL){
        printf("\n%s: Error received parameter\n",__FUNCTION__);
        return TABLES_PARSE_OK;
    }

    printf("\n********************TDT TABLE SECTION*************************");
    printf("table_id           |       %d\n",tdtTable->table_id);
    printf("section_length     |       %d\n",tdtTable->section_length);
    printf("mjd code           |       %d\n",tdtTable->mjd);

    switch(tdtTable->wday%7){
        case 1:
                printf("INFO: Time read from stream: Monday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 2:
                printf("INFO: Time read from stream: Tuesday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 3:
                printf("INFO: Time read from stream: Wednesday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 4:
                printf("INFO: Time read from stream: Thursday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 5:
                printf("INFO: Time read from stream: Friday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 6:
                printf("INFO: Time read from stream: Saturday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
        case 7:
                printf("INFO: Time read from stream: Sunday/%hu/%hu \n", tdtTable->primM, tdtTable->year);
                break;
    }

    printf("***************************TDT TABLE SECTION***********************\n", );

return TABLES_PARSE_OK;
}

ParseErrorCode parseTotTable(const uint8_t totSectionBuffer, TotTable* totTable){

    uint8_t lower8Bits=0;
    uint8_t higher8Bits=0;
    uint16_t all16Bits=0;
    uint16_t descriptors_loop_length=0;
    uint8_t loop_length_info=0;
    totTable->desc_cnt=0;

    if(totSectionBuffer==NULL || totTable==NULL){
        printf("\n%s,ERROR received parameters are not ok\n",__FUNCTION__);
        return TABLES_PARSE_OK;
    }
    totTable->table_id=(uint8_t)* totSectionBuffer;

    higher8Bits=(uint8_t) *(totSectionBuffer+1);
    lower8Bits=(uint8_t) *(totSectionBuffer+2);
    all16Bits=(uint16_t) ((higher8Bits<<8)+lower8Bits);
    totTable->sectionLength=all16Bits & 0xFFF;

    higher8Bits=(uint8_t) *(totSectionBuffer+3);
    lower8Bits=(uint8_t) *(totSectionBuffer+4);
    all16Bits=(uint16_t) ((higher8Bits<<8)+lower8Bits);
    totTable->mjd=all16Bits;

    higher8Bits=(uint8_t) *(totSectionBuffer+8);
    lower8Bits=(uint8_t) *(totSectionBuffer+9);
    all16Bits=(uint8_t) ((higher8Bits<<8)+lower8Bits);
    totTable->descriptors_loop_length=all16Bits & 0xFFF;
    descriptors_loop_length=all16Bits & 0x0FFF;

    while(descriptors_loop_length>4){
        uint8_t i=0;
        uint8_t currentDescriptor=totTable->desc_cnt;
        uint8_t currentTag=(uint8_t) *(totSectionBuffer +10 + 15*currentDescriptor);

        if(currentTag==0x58){
            totTable->dsc[currentDescriptor].descriptor_tag=currentTag;
            totTable->dsc[currentDescriptor].descriptor_length=(uint8_t) *(totSectionBuffer + 11 + 15*currentDescriptor);
            totTable->dsc[currentDescriptor].info_cnt=(uint8_t) (*(totSectionBuffer+11+15*currentDescriptor)/13);

            for(i=0; i<totTable->dsc[currentDescriptor].info_cnt;i++){
                totTable->dsc[currentDescriptor].lto_info[i].cCh1=(uint8_t) *(totSectionBuffer+12+i*13);
                totTable->dsc[currentDescriptor].lto_info[i].cCh2=(uint8_t) *(totSectionBuffer+13+i*13);
                totTable->dsc[currentDescriptor].lto_info[i].cCh3=(uint8_t) *(totSectionBuffer+14+i*13);

                lower8Bits=(uint8_t) *(totSectionBuffer+15+i*13);
                totTable->dsc[currentDescriptor].lto_info[i].lt_polarity=lower8Bits & 0x01;
                totTable->dsc[currentDescriptor].lto_info[i].region=lower8Bits>>2;

                higher8Bits=(uint8_t) *(totSectionBuffer+ 16 + i*13);
                lower8Bits=(uint8_t) *(totSectionBuffer+ 17 + i*13);
                totTable->dsc[currentDescriptor].lto_info[i].lt_hours=(higher8Bits & 0x0F) + 10*((higher8Bits & 0xF0)>>4);
                totTable->dsc[currentDescriptor].lto_info[i].lt_min=(lower8Bits & 0x0F) +10*((lower8Bits & 0xF0)>>4);

            }
        }
        totTable->desc_cnt++;
        descriptors_loop_length-=(totTable->dsc[currentDescriptor].descriptor_length+2);
    }
      return TABLES_PARSE_OK;
}

ParseErrorCode printTotTable(TotTable* totTable){
    uint8_t i=0;
    uint8_t j=0;

    if(totTable==NULL){
        printf("\n%s : Error received parameter is not ok\n",__FUNCTION__);
        return TABLES_PARSE_ERROR;
    }

    printf("\n *****************************TOT TABLE SECTION**********************\n");
    printf("table_id                |         %.2x\n",totTable->table_id);
    printf("section_length          |         %d\n",totTable->section_length);
    printf("MJD code                |         %d\n",totTable->mjd);

    for(i=0;i<totTable->desc_cnt;i++){
        printf("\n%d. descriptor: \n",i++){
        printf("descriptor_tag             |        %.2x\n",totTable->dsc[i].descriptor_tag);
        printf("descriptor_length          |        %d\n",totTable->dsc[i].descriptor_length);

        for(j=0;j<totTable->dsc[i].info_cnt;j++){
            printf("#########################################\n");
            printf("%d. LTO info                             \n",j+1);
            printf("country code          |         %.1x%.1x%.1x\n",totTable->dsc[i].lto_info[j].cCh1,totTable->dsc[i].lto_info[j].cCh2,totTable->dsc[i].lto_info[j].cCh3);
            printf("country region id     |         %d\n",totTable->dsc[i].lto_info[j].region);
            printf("local_time_polarity   |         %d\n",totTable->dsc[i].lto_info[j].lt_polarity);
            printf("local time offset     |         %.2d:%2d\n", totTable->dsc[i].lto_info[j].lt_hours,totTable->dsc[i].lto_info[j].lt_min);
        }
    }

    printf("\n************************TOT TABLE SECTION*********************\n");

    return TABLES_PARSE_OK;
}
