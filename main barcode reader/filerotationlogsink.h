#ifndef FILEROTATIONLOGSINK_H
#define FILEROTATIONLOGSINK_H

#include <QFile>
#include <QTextStream>

namespace libLogging {

class FileRotationLogSink
{
public:

    explicit FileRotationLogSink(size_t Limit = 5 * (1 << 10),QString logpath = "log" , QString bkpath = "bak");
    QTextStream &GetCurrentStream(size_t required = 0);

private:

    size_t m_Limit;
    QString m_LogFilePath, m_BackupPath;
    QTextStream m_CurrentStream;
    QFile m_File;

    void openStream();
};
}

#endif // FILEROTATIONLOGSINK_H
