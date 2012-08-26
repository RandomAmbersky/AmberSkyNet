#include "CFileSourceZIP.h"
#include "ILog.h"
#include "os_deps.h"

#include "ASN_utils.h"

#include "CFileZIP.h"

//#define local
#include "lib/minizip/unzip.h"

#define ENGINE Engine //у нас уже есть указатель на Engine

//#ifdef WIN32
//#define USEWIN32IOAPI
//#include "iowin32.h"
//#endif

int do_list(unzFile uf)
{
    uLong i;
    unz_global_info gi;
    int err;

    err = unzGetGlobalInfo (uf,&gi);
    if (err!=UNZ_OK)
    printf("error %d with zipfile in unzGetGlobalInfo \n",err);
    printf("Entry %lu\n",gi.number_entry);
    printf(" Length  Method   Size  Ratio   Date    Time   CRC-32     Name\n");
    printf(" ------  ------   ----  -----   ----    ----   ------     ----\n");
    for (i=0;i<gi.number_entry;i++)
    {

        char filename_inzip[1024];
        unz_file_info file_info;
        uLong ratio=0;
        const char *string_method;
        char charCrypt=' ';
        err = unzGetCurrentFileInfo(uf,&file_info,filename_inzip,1000,NULL,0,NULL,0);
        if (err!=UNZ_OK)
        {
            printf("error %d with zipfile in unzGetCurrentFileInfo\n",err);
            break;
        }
        if (file_info.uncompressed_size>0)
            ratio = (file_info.compressed_size*100)/file_info.uncompressed_size;

        /* display a '*' if the file is crypted  */
        if ((file_info.flag & 1) != 0)
            charCrypt='*';

        if (file_info.compression_method==0)
            string_method="Stored";
        else
        if (file_info.compression_method==Z_DEFLATED)
        {
            uInt iLevel=(uInt)((file_info.flag & 0x6)/2);
            if (iLevel==0)
              string_method="Defl:N";
            else if (iLevel==1)
              string_method="Defl:X";
            else if ((iLevel==2) || (iLevel==3))
              string_method="Defl:F"; /* 2:fast , 3 : extra fast*/
        }
        else
            string_method="Unkn. ";

        printf("%7lu  %6s%c%7lu %3lu%%  %2.2lu-%2.2lu-%2.2lu  %2.2lu:%2.2lu  %8.8lx   %s\n",
                file_info.uncompressed_size,string_method,
                charCrypt,
                file_info.compressed_size,
                ratio,
                (uLong)file_info.tmu_date.tm_mon + 1,
                (uLong)file_info.tmu_date.tm_mday,
                (uLong)file_info.tmu_date.tm_year % 100,
                (uLong)file_info.tmu_date.tm_hour,(uLong)file_info.tmu_date.tm_min,
                (uLong)file_info.crc,filename_inzip);

        if ((i+1)<gi.number_entry)
        {
            err = unzGoToNextFile(uf);
            if (err!=UNZ_OK)
            {
                printf("error %d with zipfile in unzGoToNextFile\n",err);
                break;
            }
        }
    }

    return 0;
}

CFileSourceZIP::CFileSourceZIP(IEngine *_Engine):CBaseObject(_Engine){
//Engine=_Engine;

    SetParam("BaseClass","FileSource");
    SetParam("Type","FSZIP");

LOGGER->LogMsg("+CFileSourceZIP"); //менеджер файловой системы создан
};

CFileSourceZIP::~CFileSourceZIP(){
LOGGER->LogMsg("-CFileSourceZIP");
};

int CFileSourceZIP::Release(){
    delete this;
    return 0;
};

IFile *CFileSourceZIP::LoadFile(const std::string& FileName){
int i;

if ( GetProtocolStr( FileName )!="") return NULL; // not for me

fprintf(stdout,"ZIP! try find %s\n",FileName.c_str());

std::vector<std::string>::iterator it=Paths.begin();
std::map<std::string,std::string>::iterator _it;

 for (i=Paths.size(); i>0; i--){

   if ( (_it=ZipFileSystem.find((*it)+FileName))!=ZipFileSystem.end()) {
      //// файл нашли
         fprintf(stdout,"ZIP! %s found in %s\n",( (*it)+FileName).c_str(), (*_it).second.c_str() );
/*
         unzFile uf=unzOpen( (*_it).second.c_str() );
         if (uf==NULL) continue;
         if(unzLocateFile(uf, ((*it)+FileName).c_str() ,1) != UNZ_OK) {
	       printf("file %s is not found in archive %s\n",((*it)+FileName).c_str(),(*_it).second.c_str());
		   continue;}
*/
	       CFileZIP *myZip=new CFileZIP(this);
	       myZip->FileName=((*it)+FileName).c_str();
	       myZip->ZipFileName=(*_it).second.c_str();
//         unz_file_info info;
//         unzGetCurrentFileInfo( uf ,&info,NULL,0,NULL,0,NULL,0);
//         unzCloseCurrentFile(uf);
         myZip->FileOpen();
         return myZip;
       }
     it++;
    };

/*
FILE *my=fopen(FileName.c_str(),"rb");
if (my) {
    CFileZIP *myFile=new CFileZIP(this);
    myFile->filename=FileName;
    myFile->p_File=my;
    myFile->FileOpen(); //открываем его
    return myFile; //пока :)
}
*/
return NULL; //такого файла в этом источнике нет
};

void CFileSourceZIP::AddPath(const std::string& PathName, bool Recursive){
    os_finddata_t fdata;
	os_dir_t hFile;

	unz_file_info info;
	char FileName[1024];

//printf("ZIP! add path %s\n",PathName.c_str());
Paths.push_back(PathName);

	hFile = os_findfirst((PathName).c_str(), &fdata);
	if (!hFile) return;

do
	{

	if (fdata.name=="..") continue;
	if (fdata.name==".") continue;

	if (fdata.subdir){
     if (Recursive) {
//     printf("scan dir: %s\n", fdata.name.c_str());
     std::string NewPath=fdata.dir_name+fdata.name;
     AddPath(NewPath,true); }
     }
    else {
    unzFile uf=NULL;
    std::string TempName=fdata.dir_name+fdata.name;

    uf = unzOpen((fdata.dir_name+fdata.name).c_str());

    if (uf!=NULL) {
        // файл открыли

do {

if (unzGetCurrentFileInfo(uf,&info,FileName,sizeof(FileName),NULL,0,NULL,0)!=UNZ_OK)
{
    fprintf(stderr,"Oblom-2!\n");
	break;
}

if (FileName[strlen(FileName)-1]!='/') {
ZipFileSystem[fdata.dir_name+std::string(FileName)]=fdata.dir_name+fdata.name;
fprintf(stdout,"[%s]=%s\n",(fdata.dir_name+std::string(FileName)).c_str(),TempName.c_str());
}

} while (unzGoToNextFile(uf)==UNZ_OK);

   unzCloseCurrentFile(uf);
    }

         }

	} while (os_findnext( hFile, &fdata )!= NULL);
	os_findclose(hFile);

//Paths.push_back(PathName);
};


