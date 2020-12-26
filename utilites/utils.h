#ifndef UTILS_H
#define UTILS_H

#include <string>

#include <QString>
#include <QDir>

std::string toHex(const std::string &data);

QString toHex(const QString &data);

std::string toBase64(const std::string &value);

QString toBase64(const QString &value);

std::string fromBase64(const std::string &value);

QString fromBase64(const QString &value);

std::string fromHex(const std::string &value);

QString fromHex(const QString &data);

bool isDecimal(const std::string &str);

bool isHex(const std::string &str);

std::string toLower(const std::string &str);

std::string trim(const std::string &str);

void writeToFile(const QString &pathToFile, const std::string &data, bool isCheck);

void writeToFileBinary(const QString &pathToFile, const std::string &data, bool isCheck);

std::string readFile(const QString &pathToFile);

std::string readFileBinary(const QString &pathToFile);

bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath, bool isDirs = true, bool isReplace = false);

void copyFile(const QString &srcFilePath, const QString &tgtFilePath, bool isReplace);

void copyToDirectoryFile(const QString &srcFilePath, const QString &tgtDirectory, bool isReplace);

void createFolder(const QString &folder);

bool isExistFile(const QString &file);

bool isExistFolder(const QString &folder);

void removeFile(const QString &file);

void removeFolder(const QString &folder);

bool isPathEquals(const QString &path1, const QString &path2);

QString getFileName(const QString &filePath);

QString getBaseName(const QString &filePath);

QString getExtension(const QString &filePath);

bool isDirectory(const QString &path);

QStringList getFilesForDir(const QString &path);

inline QString makePath(const QString &arg) {
    return arg;
}

template<typename... Args>
inline QString makePath(const QString &arg, const Args& ...args) {
    auto removeSlash = [](const QString &str) {
        if (str.startsWith('/')) {
            return str.mid(1);
        } else {
            return str;
        }
    };
    return QDir(arg).filePath(removeSlash(makePath(args...)));
}

#endif // UTILS_H
