/** @file   find_big_file.cpp
 *
 *  @brief  查找指定目录的大文件
 *  @note
 *  @author lesliefish
 *  @date   2020/03/17
 */
#include <QCoreApplication>
#include <QFileInfo>
#include <QDir>
#include <map>
#include <iostream>

using namespace std;

#define MAX_SIZE 1024 * 1024 * 100

/*
 * @func   findBigFile 
 * @brief  函数接口
 * @param  [in]  const QString & strDirPath  目录路径
 * @param  [out]  multimap<qint64, QString> & infoMap 文件路径及大小存入map 
 * @return void  
 */ 
void findBigFile(const QString& strDirPath, multimap<qint64, QString>& infoMap)
{
    QDir dir(strDirPath);

    QFileInfoList list = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (int i = 0; i < list.size(); ++i)
    {
        if (list[i].isFile())
        {
            if (list[i].size() > MAX_SIZE)
            {
                cout << list[i].absoluteFilePath().toLocal8Bit().data() << endl;
                infoMap.insert(pair<qint64, QString>(list[i].size()/1024/1024, list[i].absoluteFilePath()));
            }
        }
        else
        {
            findBigFile(list[i].absoluteFilePath(), infoMap);
        }
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dirPath("D:\\");

    multimap<qint64, QString> fileMap;

    findBigFile(dirPath, fileMap);

    for (auto riter = fileMap.rbegin(); riter != fileMap.rend(); ++riter)
    {
        cout << riter->second.toLocal8Bit().data() << endl;
    }

    return a.exec();
}
