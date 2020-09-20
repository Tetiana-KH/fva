#include "fvacommoncsv.h"
#include "fvadescriptionfile.h"
#include "fvadefaultcfg.h"
#include "data/fvafile.h"
#include "data/fvadevice.h"

#include <QtCore/QTextStream>
#include <QtCore/QFile>

FVA_EXIT_CODE fvaGetIDFromFile(const QString& fileName, int& ID)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return FVA_ERROR_CANT_OPEN_ID_FILE;
	QTextStream readStream(&file);
	readStream >> ID;
	file.close();
	return FVA_NO_ERROR;
}

FVA_EXIT_CODE fvaSaveIDInFile(const QString& fileName, int ID)
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;
	QTextStream writeStream(&file);
	writeStream << ID;
	writeStream.flush();
	file.close();
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadFvaFileInfoFromCsv(FVA_FILE_INFO_MAP& fvaFileInfo)
{
	FVADescriptionFile fvaFileCsv;

	// firstly - try to get device if from fvaFile.csv as it has high priority 
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;

	FVA_EXIT_CODE res = fvaFileCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR

		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "DevId");
	if (-1 == columnDevId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnID = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		QString fileName = list[columnName].toUpper();
		if (fvaFileInfo.find(fileName) != fvaFileInfo.end())
		{
			QFile file(FVA_DEFAULT_ROOT_DIR + "#data#/fvaNotUniqueFileName.csv");
			file.open(QIODevice::WriteOnly | QIODevice::Append);
			QTextStream writeStream(&file);
			writeStream << list[columnID].toUpper() << "\n";
			file.close();
			return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
		fvaFile newFile;
		newFile.deviceId = list[columnDevId].remove("\t").toUInt();
		fvaFileInfo[fileName.toUpper()] = newFile;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaGetDeviceIdFromCsv(const FVA_FILE_INFO_MAP& fvaFileInfo, const QString& fvaFile, int& deviceID, const QString& dir)
{
	deviceID = FVA_UNDEFINED_ID;

	if (fvaFileInfo.find(fvaFile.toUpper()) != fvaFileInfo.end())
	{
		deviceID = fvaFileInfo[fvaFile.toUpper()].deviceId;
		return FVA_NO_ERROR;
	}

	// we did not find it on fvafile info, lets try to find it in folder fva info
	FVADescriptionFile fvaFolderCsv;
	QStringList			titlesD;
	DESCRIPTIONS_MAP	decsItemsD;
	FVA_EXIT_CODE res = fvaFolderCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.csv", titlesD, decsItemsD);
	RET_RES_IF_RES_IS_ERROR

	// ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder,WhoTookFotoId,Scanerid
	int columnDevIdD = FVADescriptionFile::getColumnIdByName(titlesD, "DevId");
	if (-1 == columnDevIdD)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnNameD = FVADescriptionFile::getColumnIdByName(titlesD, "Name");
	if (-1 == columnNameD)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	QString dirToMatch = dir;
	dirToMatch = dirToMatch.replace("\\", "/");  // replace slaches on backslashes
	dirToMatch = dirToMatch.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir
	dirToMatch = "/" + dirToMatch;

	for (DESCRIPTIONS_MAP::Iterator it = decsItemsD.begin(); it != decsItemsD.end(); ++it)
	{
		QStringList list = it.value();

		if (list[columnNameD].toUpper() == dirToMatch.toUpper())
		{
			if (deviceID == FVA_UNDEFINED_ID)
			{
				deviceID = list[columnDevIdD].remove("\t").toUInt();

				QFile file(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFileNoDevId.csv");
				file.open(QIODevice::WriteOnly | QIODevice::Append);
				int ID = FVA_UNDEFINED_ID;
				fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.id", ID);
				QTextStream writeStream(&file);
				writeStream << QString::number(++ID) << "," << fvaFile << ",,," << QString::number(deviceID) << ",,,,,,\n";
				fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFile.id", ID);
				file.close();

				return FVA_NO_ERROR;
			}
			else
				return FVA_ERROR_NON_UNIQUE_FVA_INFO;
		}
	}

	return FVA_ERROR_NO_DEV_ID;
};
FVA_EXIT_CODE fvaLoadDeviceMapFromCsv(DEVICE_MAP& deviceMap)
{
	FVADescriptionFile	fvaDeviceCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaDeviceCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaDevices.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,OwnerId,LinkedName,Name,fvaDeviceType
	int columnDevId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnDevId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnOwnerId = FVADescriptionFile::getColumnIdByName(titles, "OwnerId");
	if (-1 == columnOwnerId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnLinkedName = FVADescriptionFile::getColumnIdByName(titles, "LinkedName");
	if (-1 == columnLinkedName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnfvaDeviceType = FVADescriptionFile::getColumnIdByName(titles, "fvaDeviceType");
	if (-1 == columnfvaDeviceType)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaDevice device;
		device.linkedName	= list[columnLinkedName].remove("\t").trimmed();
		device.deviceId		= list[columnDevId].remove("\t").toUInt();
		device.guiName		= list[columnName].remove("\t").trimmed();
		device.ownerName	= "N/A";
		device.ownerId		= list[columnOwnerId].remove("\t").toUInt();
		device.type			= static_cast<FVA_DEVICE_TYPE> (list[columnfvaDeviceType].remove("\t").toUInt());
		deviceMap[device.deviceId] = device;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaLoadPeopleMapFromCsv(PEOPLE_MAP& peopleMap)
{
	FVADescriptionFile	fvaPeopleCsv;
	QStringList			titles;
	DESCRIPTIONS_MAP	decsItems;
	FVA_EXIT_CODE res = fvaPeopleCsv.load(FVA_DEFAULT_ROOT_DIR + "#data#/fvaPeople.csv", titles, decsItems);
	RET_RES_IF_RES_IS_ERROR
	// ID,Name,FullName,RelationId,RelPersonID
	int columnId = FVADescriptionFile::getColumnIdByName(titles, "ID");
	if (-1 == columnId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnName = FVADescriptionFile::getColumnIdByName(titles, "Name");
	if (-1 == columnName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnFullName = FVADescriptionFile::getColumnIdByName(titles, "FullName");
	if (-1 == columnFullName)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnRelationId = FVADescriptionFile::getColumnIdByName(titles, "RelationId");
	if (-1 == columnRelationId)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	int columnRelPersonID = FVADescriptionFile::getColumnIdByName(titles, "RelPersonID");
	if (-1 == columnRelPersonID)
		return FVA_ERROR_CANT_FIND_MANDATORY_FIELDS;

	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		QStringList list = it.value();

		fvaPerson person;
		person.Id				= list[columnId].remove("\t").toUInt();
		person.name				= list[columnName].remove("\t").trimmed();		
		person.fullName			= list[columnFullName].remove("\t").trimmed();
		person.relatedPersionId	= list[columnRelPersonID].remove("\t").toUInt();
		person.relationId		= list[columnRelationId].remove("\t").toUInt();
		peopleMap[person.Id]	= person;
	}
	return FVA_NO_ERROR;
}
FVA_EXIT_CODE fvaUpdateFvaDirInfoInCsv(const QString& dirPath)
{
	int ID = FVA_UNDEFINED_ID;
	FVA_EXIT_CODE res = fvaGetIDFromFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.id", ID);
	RET_RES_IF_RES_IS_ERROR

	QString dir = dirPath; 
	dir.replace("\\", "/");  // replace slaches on backslashes
	dir = dir.remove(FVA_DEFAULT_ROOT_DIR); // remove a prefix as root dir

	QFile fileNew(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolderN.csv");
	if (!fileNew.open(QIODevice::WriteOnly | QIODevice::Text))
		return FVA_ERROR_CANT_OPEN_NEW_DIR_DESC;

	QTextStream writeStream(&fileNew);
	//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder,WhoTookFotoId,Scanerid
	writeStream << QString::number(++ID) << ",/" << dir << ",0,,,,,,,," << "\n";
	writeStream.flush();
	
	fileNew.close();

	return fvaSaveIDInFile(FVA_DEFAULT_ROOT_DIR + "#data#/fvaFolder.id", ID);
}