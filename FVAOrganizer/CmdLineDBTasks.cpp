#include "CmdLineDBTasks.h"
#include "FVADescriptionFile.h"

void CLT_Base_SQL::SaveSQL( const QString& fileToSaveIn )
{
	QFile fileNew ( m_folder + QDir::separator() + fileToSaveIn );	
	if ( !fileNew.open( QIODevice::Append | QIODevice::Text ) )
	{
		LOG_QCRIT << "can not create fva sql for " << m_folder;
		return;
	}
	QTextStream writeStream( &fileNew );
	writeStream.setCodec("UTF-8");
	writeStream << "BEGIN TRANSACTION;" << "\n";
	for ( auto it = m_SQLs.begin(); it != m_SQLs.end(); ++it )
		writeStream << *it << "\n";	
	writeStream << "COMMIT;";
	writeStream.flush();
	fileNew.close();
}

CLT_Fs_To_SQL::~CLT_Fs_To_SQL()
{
	SaveSQL("11.fva.sql");
	qDebug() << "totally inserted - " << m_SQLs.size() << ", totally skipped - " << m_skippedFiles;
}
FVA_ERROR_CODE CLT_Fs_To_SQL::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		// just skip internal folder
		if ( ( info.isDir() && info.fileName()[0] == '#' && info.fileName()[info.fileName().size()-1] == '#' )
			|| 
			info.isFile() && !fvaIsFVAFile ( info.suffix().toUpper()))
		{
			qDebug() << "skipped internal fs object - " << info.absoluteFilePath() ;
			m_skippedFiles++;
			continue;
		}
		//ID,Name,Path,Type,ReadOnly,Size,DataModified,DataCreated,FvaFileId,FvaFolderId,FsImageId,FsVideoId,FsAudioId
#define SPRT ","
		QString insert =  "insert into fva values ((select max(ID)+1 from fva),\"" // ID
			+ info.fileName().toUpper() + "\"" + SPRT // Name
			+ "\"" + info.absolutePath().toUpper() + "\"" + SPRT // Path
			+ QString::number(info.isDir()?FVA_FS_TYPE_DIR:fvaConvertFileExt2FileType(info.suffix().toUpper())) + SPRT // Type
			+ (info.isWritable() ? "0":"1") + SPRT // ReadOnly
			+ QString::number(info.size()) + SPRT // Size
			+ "\"" + info.lastModified().toString("yyyy-MM-dd hh:mm:ss.zzz") + "\"" + SPRT // DataModified
			+ "\"" + info.created().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) + "\"" + SPRT // DataCreated
			+ "-1" + SPRT // FvaFileId
			+ "-1" + SPRT // FvaFolderId 
			+ "-1" + SPRT // FsImageId 
			+ "-1" + SPRT // FsVideoId
			+ "-1"		  // FsAudioId
			+ ");";
		m_SQLs.push_front(insert);
	}
	return FVA_NO_ERROR;
}
CLT_Fva_Folder_To_SQL::~CLT_Fva_Folder_To_SQL()
{
	SaveSQL( "12.fvaFolder.sql" );
	
	for ( auto it = m_movedFolders.begin(); it != m_movedFolders.end(); ++it )
	{
		QFileInfo info( *it );
		if ( !m_dir.rename( *it + QDir::separator() + FVA_DIR_DESCRIPTION_FILE_NAME, 
						m_folder + QDir::separator() + info.fileName() + ".json" ))
		{
			LOG_QCRIT << "can not move fva desc for " << *it;
			continue;
		}
		LOG_QWARN	<< "moved from - "	<< *it + QDir::separator() + FVA_DIR_DESCRIPTION_FILE_NAME
					<< " to - "			<< m_folder + QDir::separator() + info.fileName() + ".json";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFolders.size();	
}
QString DVget( const QString& fieldName, QVariantMap& result )
{
	QString fieldValue = "";
	if ( result.contains( fieldName ) )
	{
		fieldValue = result[fieldName].toString();
		result.remove(fieldName);
	}
	return fieldValue;
}
FVA_ERROR_CODE CLT_Fva_Folder_To_SQL::execute()
{
	QVariantMap		dirDesc;
	QString			error;
	FVA_ERROR_CODE	code = fvaGetFolderDescription( m_folder, dirDesc, error );
	if ( FVA_NO_ERROR != code )
		return FVA_NO_ERROR; // yes, it is NOT an error for this CLT

	//ID,Name,DevId,Tags,People,PlaceId,EventId,ReasonPeople,LinkedFolder
	QString insert =  "insert into fvaFolder values ((select max(ID)+1 from fvaFolder),\"" 
		+ m_dir.dirName()					+ "\"," 
		+ DVget( "deviceId", dirDesc)		+ ",\""
		+ DVget( "tags", dirDesc )			+ "\",\""
		+ DVget( "people", dirDesc )		+ "\",\""
		+ DVget( "place", dirDesc )			+ "\",\""
		+ DVget( "event", dirDesc )			+ "\",\""
		+ DVget( "reasonPeople", dirDesc )	+ "\",\""
		+ DVget( "linkedFolder", dirDesc )	+ "\",\""
		+ DVget( "whoTookFotoId", dirDesc )	+ "\",\""
		+ DVget( "scaner", dirDesc)			+ "\");";
	m_SQLs.push_back(insert);	
	QString update;
	QFileInfo info(m_folder);
	update = "update fva set FvaFolderId = (select max(ID) from fvaFolder) where Path || \"/\" ||  Name = \"" + info.absoluteFilePath() + "\";";
	m_SQLs.push_back(update);
	
	if ( dirDesc.size() )
	{
		return FVA_ERROR_INCORRECT_FORMAT;
	}

	// move dir description file for future backup purpose
	m_movedFolders.push_back( m_folder );
	LOG_QWARN << "converted folder description to SQL:" << m_folder;
	return FVA_NO_ERROR;
}
CLT_Fva_File_To_SQL::~CLT_Fva_File_To_SQL()
{
	SaveSQL( "13.fvaFile.sql" );

	for ( auto it = m_movedFiles.begin(); it != m_movedFiles.end(); ++it )
	{
		QFileInfo info( *it );
		if ( !m_dir.rename( *it + QDir::separator() + FVA_DESCRIPTION_FILE_NAME, 
						m_folder + QDir::separator() + info.fileName() + ".csv" ))
		{
			LOG_QCRIT << "can not move file fva desc for " << *it;
			continue;
		}
		LOG_QWARN	<< "moved from - "	<< *it + QDir::separator() + FVA_DESCRIPTION_FILE_NAME
					<< " to - "			<< m_folder + QDir::separator() + info.fileName() + ".csv";
	}

	LOG_QWARN << "totally inserted - " << m_SQLs.size() / 2 << ", totally moved - " << m_movedFiles.size();
}
QString FVget( FVADescriptionFile* desc, const QString& fieldName, const QStringList& titles, DESCRIPTIONS_MAP::Iterator it, QStringList& titlesToCheck )
{
	int indexColumn = desc->getColumnIdByName(titles,fieldName);
	if ( -1 == indexColumn )
	{
		// qCritical() << "could not find " << fieldName << " column in description file";
		return "";
	}
	titlesToCheck[ indexColumn ] = "";
	return it.value()[ indexColumn ];
}

FVA_ERROR_CODE CLT_Fva_File_To_SQL::execute()
{
	std::auto_ptr<FVADescriptionFile> desc( new FVADescriptionFile );
	QStringList			titles; 
	DESCRIPTIONS_MAP	decsItems;
	FVA_ERROR_CODE res = desc->load( m_folder + QDir::separator() + FVA_DESCRIPTION_FILE_NAME, titles, decsItems );
	if ( FVA_NO_ERROR != res )
		return FVA_NO_ERROR; // yes, it is NOT an error for this CLT
	
	QStringList			t2Check = titles; // titlesToCheck 
	for (DESCRIPTIONS_MAP::Iterator it = decsItems.begin(); it != decsItems.end(); ++it)
	{
		// ID,Name,PlaceId,People,DevId,Description,ScanerId,Comment,OldName,WhoTook,OldName1
		QString insert =  "insert into fvaFile values ((select max(ID)+1 from fvaFile),\""
		+ FVget( desc.get(),"Name",titles, it, t2Check ).toUpper()		+ "\",\""
		+ FVget( desc.get(),"Place",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"People",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Device",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Description",titles, it, t2Check )	+ "\",\""
		+ FVget( desc.get(),"Scaner",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"Comment",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"oldName",titles, it, t2Check ).toUpper()		+ "\",\""
		+ FVget( desc.get(),"WhoTook",titles, it, t2Check )		+ "\",\""
		+ FVget( desc.get(),"oldName1",titles, it, t2Check ).toUpper()	+ "\");";

		m_SQLs.push_back( insert );

		QString update;
		QFileInfo info(m_folder);
		update = "update fva set FvaFileId = (select max(ID) from fvaFile) where Path || \"/\" ||  Name = \"" 
				+ info.absoluteFilePath ().toUpper()
				+ "/" 
				+ FVget( desc.get(),"Name",titles, it, t2Check ).toUpper() 
				+ "\";";
		m_SQLs.push_back(update);
	}

	for ( int id = 0; id < t2Check.size(); ++id )
	{
		if ( t2Check[ id ].length() > 0 )
			return FVA_ERROR_INCORRECT_FORMAT;
	}
	// move dir description file for future backup purpose
	m_movedFiles.push_back( m_folder );
	
	LOG_QWARN << "converted file description to SQL:" << m_dir.dirName();
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fsaudio_To_SQL::execute()
{
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fsvideo_To_SQL::execute()
{
	return FVA_NO_ERROR;
}

FVA_ERROR_CODE CLT_Fsimage_To_SQL::execute()
{
	return FVA_NO_ERROR;
}