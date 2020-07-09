#include "CmdLineOldTasks.h"

#include "../lib/qexifimageheader.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QCryptographicHash>

FVA_ERROR_CODE CLT_Dir_Struct_Create_By_Device_Name::execute()
{
	QString deviceName;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		// if it is picture files
		QString suffix = info.suffix().toUpper();
		QString fullname = info.absoluteFilePath();
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
		{
			QString newDeviceName = QExifImageHeader( info.filePath()).value(QExifImageHeader::Make).toString()
									+ QExifImageHeader( info.filePath()).value(QExifImageHeader::Model).toString();
			
			while(newDeviceName.endsWith( '\n' )) 
				newDeviceName.chop( 1 );

			while(newDeviceName.endsWith( '\r' ))
				newDeviceName.chop( 1 );

			while(newDeviceName.endsWith( QChar( '\0' )))
				newDeviceName.chop( 1 );

			newDeviceName = newDeviceName.trimmed();
				
			QString dirName;
			if ( newDeviceName.isEmpty() )
			{
				LOG_QDEB << "no device name in picture:" << info.absoluteFilePath();
				dirName = "EMPTY";
			}
			else
			{
				if ( deviceName.isEmpty() )
					LOG_QDEB << "got first device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath();
				else if ( deviceName != newDeviceName )	
					LOG_QWARN << "got new device name: "<< newDeviceName << "in picture:" << info.absoluteFilePath() << "old: " << deviceName;
						
				deviceName = newDeviceName;
				dirName = deviceName;
			}

			QString fullSubFolderpath	=  m_folder + "/" + dirName;

			if ( !m_dir.exists( dirName ) )
			{
				m_dir.mkdir( dirName );
				LOG_QDEB << "sub-folder:" << dirName << " created";
			}
			// move the file
			if ( !m_dir.rename( info.absoluteFilePath(), fullSubFolderpath + "/" + info.fileName() ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
				LOG_QDEB << "file renamed:" << info.absoluteFilePath() << " into:" << fullSubFolderpath + "/" + info.fileName();
		}
		else
			LOG_QDEB << " no device name in:" << info.absoluteFilePath();
	}
	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Files_Rename_By_Dir::execute()
{
	int id = 0;
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;
		QString suffix = info.suffix().toUpper();
		if(	FVA_FS_TYPE_IMG == fvaConvertFileExt2FileType ( suffix ) )
		{				
			QString newPath = m_dir.path() + "/" + m_dir.dirName().replace(".","-").mid(0,10) + "-##-##-" + QString::number( id ) + "." + info.suffix();
			if ( !m_dir.rename( info.absoluteFilePath(), newPath ) )
			{
				LOG_QCRIT << "can not rename file:" << info.absoluteFilePath() << " to:" << newPath;
				return FVA_ERROR_CANT_RENAME_FILE;
			}
			else
			{
				LOG_QDEB << "renamed file:" << info.absoluteFilePath() << " to:" << newPath;
				id++;
			}
		}	
	}

	return FVA_NO_ERROR;
}
FVA_ERROR_CODE CLT_Dir_Name_Change::execute()
{
	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
		{
			QString subFolderName		= info.baseName().mid(0,10);
			info.absoluteDir();
			QDir subdir ( info.absoluteDir() );

			QString oldPath = info.absoluteFilePath();
			QString newPath = info.absoluteFilePath().replace("-",".");
			if ( subdir.rename( oldPath, newPath ) )
				LOG_QWARN << "renamed folder from " << oldPath << " to " << newPath;
			else
				LOG_QCRIT << "cannot rename folder from " << oldPath << " to " << newPath;			
		}
		else
			continue;			
	}

	return FVA_NO_ERROR;
}
CLT_Print_FS_Structure::CLT_Print_FS_Structure(const QString& dir_,bool readOnly_,const QString& custom_)
	:CmdLineBaseTask( dir_,readOnly_,custom_)
{
	qDebug()<<"["<<Name().toUpper()<<"]cmd created,dir:"<<dir_;
	m_file.setFileName(QCoreApplication::applicationDirPath() + "\\fsoutput.txt");
	m_file.open( QIODevice::WriteOnly );
}

CLT_Print_FS_Structure::~CLT_Print_FS_Structure()
{
	m_file.close();
}

FVA_ERROR_CODE CLT_Print_FS_Structure::execute()
{
	char		buffer [ 64* 1024 ];
	qint64		size = 0;
	QCryptographicHash hash( QCryptographicHash::Sha1 );
	QString		result;

	Q_FOREACH(QFileInfo info, m_dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
	{
		if ( info.isDir() )
			continue;

		QFile file( info.absoluteFilePath() );

		if ( !file.open( QIODevice::ReadOnly ) ) 
		{
			LOG_QCRIT << "failed to open file:" << info.absoluteFilePath();
			continue;
		}
		while (!file.atEnd())
		{
			size	= file.read(buffer, 64 * 1024);
			if (size)
				hash.addData( buffer, size);
		}
		QFileInfo f(info);
		result =	info.filePath()										+ "," 
					+	hash.result().toBase64()						+ ","
					+	f.lastModified().toString("yyyy-MM-dd-hh-mm-ss")	+ ","
					+	QString::number(info.size())					+ "\n";
		m_file.write(result.toLocal8Bit());
	}
	return FVA_NO_ERROR;
}