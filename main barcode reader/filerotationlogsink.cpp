#include "filerotationlogsink.h"

libLogging::FileRotationLogSink::FileRotationLogSink(size_t Limit,QString logpath  , QString bkpath ) :
    m_Limit(Limit),
    m_LogFilePath(logpath),
    m_BackupPath(bkpath)
{
    m_File.setFileName(m_LogFilePath);
    m_File.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    m_CurrentStream.setDevice(&m_File);
}

QTextStream& libLogging::FileRotationLogSink::GetCurrentStream(size_t required) {

    if (m_File.size() + required > m_Limit) {
        m_File.flush();
        m_File.close();

        // remove old backup
        if (QFile::exists(m_BackupPath))
            QFile::remove(m_BackupPath);

        // backup current logfile
        QFile::rename(m_LogFilePath, m_BackupPath);
        m_File.open(QIODevice::WriteOnly | QIODevice::Text);
    }

    return m_CurrentStream;
}

