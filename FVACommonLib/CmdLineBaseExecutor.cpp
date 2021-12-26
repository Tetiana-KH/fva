/*!
* \file CmdLineBaseExecutor.cpp
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#include "CmdLineBaseExecutor.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>

FVA_EXIT_CODE CmdLineBaseExecutor::run(const CLTContext& context, const FvaConfiguration& cfg)
{
	QDir dir(context.dir);

	if (!dir.exists(context.dir))
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[CLTEXECUTOR]input folder does not exist";
		return FVA_ERROR_INPUT_DIR_NOT_EXIST_ARG;
	}

	std::unique_ptr<CmdLineBaseTask> task(createTaskByName(context, cfg));
	if (!task.get())
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[CLTEXECUTOR]uknown command:" << context.cmdType;
		return FVA_ERROR_UKNOWN_CMD;
	}
	qDebug() << "[DBG]" << QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[" << context.cmdType.toUpper() << "]"
		<< "cmd created,dir:" << context.dir
		<< ",RO=" << (context.readOnly ? "yes" : "no")
		<< ",REC=" << (context.recursive ? "yes" : "no");

	if (context.readOnly && !task->supportReadOnly())
	{
		qCritical() << "[ERR]" << QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[CLTEXECUTOR]command does not support readonly:" << context.cmdType;
		return FVA_ERROR_NOT_SUPPORTED_RO_MODE;
	}

	// if it is recursive command
	if (context.recursive)
	{
		qWarning() << "[DBG]" << QDateTime::currentDateTime().toString(LOG_DT_FORMAT).toLatin1().data() << "[CLTEXECUTOR] RECURSIVE mode for cmd:" << context.cmdType;
		return task->processFolderRecursivly(context.dir, context);
	}
	else
		return task->execute(context);
}