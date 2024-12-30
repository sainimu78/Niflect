#include "Niflect/Util/SystemUtil.h"
#include "Niflect/Util/StringUtil.h"//TestResolvePath
#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef _WIN32
#include <direct.h> // for _mkdir
#else
#include <sys/stat.h> // for mkdir
#endif

//#ifdef _WIN32
//#include <direct.h> // for _getcwd
//#define GETCWD _getcwd
//#include <Windows.h> // for MAX_PATH
//#define MY_PATH_MAX MAX_PATH
//#else
//#include <unistd.h> // for getcwd
//#include <limits.h>
//#define GETCWD getcwd
//#define MY_PATH_MAX PATH_MAX
//#endif

namespace NiflectUtil
{
    static bool create_directory(const Niflect::CString& path)
    {
        struct stat info;
        // 检查路径是否已存在
        if (stat(path.c_str(), &info) != 0)
        {
            // 路径不存在，尝试创建
#ifdef _WIN32
            if (_mkdir(path.c_str()) != 0)
            {
                std::cerr << "Error creating directory: " << path << std::endl;
                return false;
            }
#else
            if (mkdir(path.c_str(), 0777) != 0)
            {
                if (errno != EEXIST)
                {
                    std::cerr << "Error creating directory: " << path << std::endl;
                    return false;
                }
            }
#endif
        }
        else if (!(info.st_mode & S_IFDIR))
        {
            // 路径存在但不是目录
            std::cerr << path << " exists but is not a directory." << std::endl;
            return false;
        }
        return true;
    }
    void MakeDirectories(const Niflect::CString& file_path)
    {
        std::string::size_type pos = file_path.find_last_of('/');
        if (pos != std::string::npos) {
            auto dir = file_path.substr(0, pos);

            Niflect::TArrayNif<Niflect::CString> path_segments;
            Niflect::CString current_path;
            for (char ch : dir) {
                if (ch == '/') {
                    if (!current_path.empty()) {
                        path_segments.push_back(current_path);
                        current_path.clear();
                    }
#ifdef _WIN32
#else
                    else
                    {
                        path_segments.push_back(current_path);
                    }
#endif
                }
                else {
                    current_path += ch;
                }
            }
            if (!current_path.empty()) {
                path_segments.push_back(current_path);
            }

            Niflect::CString constructed_path;
            for (const auto& segment : path_segments) {
                constructed_path += segment + '/';
                if (!create_directory(constructed_path)) {
                    std::cerr << "Failed to create path: " << constructed_path << std::endl;
                    return;
                }
            }
        }
    }

    //Niflect::CString GetCurrentWorkingDir()
    //{
    //    char buffer[MY_PATH_MAX];
    //    char* cwd = GETCWD(buffer, sizeof(buffer));

    //    Niflect::CString result;
    //    if (cwd) {
    //        result = cwd;
    //    }
    //    else {
    //        std::cerr << "Error getting current working directory." << std::endl;
    //    }

    //    return result;
    //}

//    bool RemoveDirectory(const std::string& dir_path) {
//#ifdef _WIN32
//        // Windows下使用 rmdir 命令
//        std::string command = "rmdir /s /q \"" + dir_path + "\"";
//#else
//        // Unix/Linux/Mac下使用 rm -rf 命令
//        std::string command = "rm -rf \"" + dir_path + "\"";
//#endif
//
//        int result = std::system(command.c_str());
//        if (result == 0) {
//            //std::cout << "Directory removed successfully: " << dir_path << std::endl;
//            return true;
//        }
//        //std::cerr << "Error removing directory: " << dir_path << std::endl;
//        return false;
//    }
}

#ifdef _WIN32
#include <windows.h>
#include <algorithm>
#else
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace NiflectUtil
{
    bool DeleteDirectory(const Niflect::CString& dir) {
        if (dir.empty())
            return false;
#ifdef _WIN32
        WIN32_FIND_DATA fileData;
        HANDLE hFind;

        // Append a wildcard to find all files and directories
        auto search_path = dir + "\\*";
        hFind = FindFirstFile(search_path.c_str(), &fileData);

        if (hFind == INVALID_HANDLE_VALUE) {
            return false;  // Failed to open directory
        }

        do {
            const Niflect::CString filename = fileData.cFileName;

            // Skip the special entries "." and ".."
            if (filename == "." || filename == "..") {
                continue;
            }

            const auto full_path = dir + "\\" + filename;

            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                // Recursively delete subdirectory
                DeleteDirectory(full_path);
                RemoveDirectory(full_path.c_str());
            }
            else {
                // Delete file
                DeleteFile(full_path.c_str());
            }
        } while (FindNextFile(hFind, &fileData) != 0);

        FindClose(hFind);
        return RemoveDirectory(dir.c_str());
#else
        DIR* directory = opendir(dir.c_str());
        if (!directory) {
            return false; // Failed to open directory
        }

        struct dirent* entry;
        while ((entry = readdir(directory)) != nullptr) {
            const Niflect::CString filename = entry->d_name;

            // Skip the special entries "." and ".."
            if (filename == "." || filename == "..") {
                continue;
            }

            const Niflect::CString full_path = dir + "/" + filename;

            if (entry->d_type == DT_DIR) {
                // Recursively delete subdirectory
                DeleteDirectory(full_path);
                rmdir(full_path.c_str());
            }
            else {
                // Delete file
                remove(full_path.c_str());
            }
        }

        closedir(directory);
        return rmdir(dir.c_str()) == 0;
#endif
    }

    bool DirectoryExists(const Niflect::CString& dir) {
#ifdef _WIN32
        DWORD dwAttrib = GetFileAttributes(dir.c_str());
        return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#else
        struct stat info;
        return (stat(dir.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
#endif
    }

    bool FileExists(const Niflect::CString& path) {
#ifdef _WIN32
        DWORD attributes = GetFileAttributes(path.c_str());
        return (attributes != INVALID_FILE_ATTRIBUTES);
#else
        struct stat buffer;
        return (stat(path.c_str(), &buffer) == 0);
#endif
    }
}
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif
namespace NiflectUtil
{
    Niflect::CString GetCurrentWorkingDirPath() {
#ifdef _WIN32
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        Niflect::CString result(buffer);
        std::replace(result.begin(), result.end(), '\\', '/');
        return result;
#else
        char buffer[PATH_MAX];
		if (getcwd(buffer, sizeof(buffer)) == NULL) {
			ASSERT(false);
			perror("getcwd error");
		}
        return Niflect::CString(buffer);
#endif
    }
}
namespace NiflectUtil
{
    static bool IsRelativePath(const Niflect::CString& path) {
        return !(path.empty() || path[0] == '/' || (path.length() > 1 && path[1] == ':'));
    }
    Niflect::CString ResolvePath(const Niflect::CString& relativePath) {
        if (!IsRelativePath(relativePath))
            return relativePath;
        Niflect::CString absolutePath;
        auto currentPath = GetCurrentWorkingDirPath();
        Niflect::TArrayNif<Niflect::CString> parts;

        // 将当前路径分割成部分
        size_t start = 0;
        size_t end = currentPath.find('/');

        while (end != std::string::npos) {
            parts.push_back(currentPath.substr(start, end - start));
            start = end + 1;
            end = currentPath.find('/', start);
        }

        // 处理最后的部分
        parts.push_back(currentPath.substr(start));

        // 解析相对路径
        start = 0;
        end = relativePath.find('/');

        while (end != std::string::npos) {
            auto  token = relativePath.substr(start, end - start);
            if (token == "..") {
                // 上层目录，移除当前目录的最后部分
                if (!parts.empty()) {
                    parts.pop_back();
                }
            }
            else if (token != "." && !token.empty()) {
                // 当前目录或非空部分，添加到路径
                parts.push_back(token);
            }
            start = end + 1;
            end = relativePath.find('/', start);
        }

        // 处理最后的部分
        Niflect::CString token = relativePath.substr(start);
        if (token == "..") {
            if (!parts.empty()) {
                parts.pop_back();
            }
        }
        else if (token != "." && !token.empty()) {
            parts.push_back(token);
        }

        // 组合成绝对路径
        for (const auto& part : parts) {
            absolutePath += part + '/';
        }

        // 去掉最后的斜杠
        if (!absolutePath.empty()) {
            absolutePath.pop_back();
        }
        return absolutePath;
    }
#ifdef TEST_TestResolvePath
    static void TestResolvePath()
    {
        //不支持路径中间部分含 ../

        auto cw = NiflectUtil::GetCurrentWorkingDirPath();//F:/Fts/Proj/Test/aaaaaaaaa/Build
        auto cwUpper = NiflectUtil::GetParentDirPath(cw);//F:/Fts/Proj/Test/aaaaaaaaa
        Niflect::CString expected0(NiflectUtil::ConcatPath(cw, "MyClass.h"));//F:/Fts/Proj/Test/aaaaaaaaa/Build/MyClass.h
        Niflect::CString expected1(NiflectUtil::ConcatPath(cwUpper, "MyClass.h"));//F:/Fts/Proj/Test/aaaaaaaaa/MyClass.h
        Niflect::CString expected2(NiflectUtil::ConcatPath(cwUpper, "MyFolder"));//F:/Fts/Proj/Test/aaaaaaaaa/MyFolder
        auto a = NiflectUtil::ResolvePath("../MyClass.h");
        ASSERT(a == expected1);
        printf("%s\n", a.c_str());
        auto b = NiflectUtil::ResolvePath(expected0);
        ASSERT(b == expected0);
        printf("%s\n", b.c_str());
        auto c = NiflectUtil::ResolvePath("MyClass.h");
        ASSERT(c == expected0);
        printf("%s\n", c.c_str());
        auto d = NiflectUtil::ResolvePath("./MyClass.h");
        ASSERT(d == expected0);
        printf("%s\n", d.c_str());
        auto e = NiflectUtil::ResolvePath("../MyFolder");
        ASSERT(e == expected2);
        printf("%s\n", e.c_str());
    }
#endif
}


#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

namespace NiflectUtil
{
    // 检查文件名是否匹配模式
    static bool match(const Niflect::CString& filename, const Niflect::CString& pattern) {
        // 如果模式为空，则直接返回
        if (pattern.empty()) {
            return filename.empty();
    }

        // dp数组，用于动态规划
        Niflect::TArrayNif<Niflect::TArrayNif<bool> > dp(filename.size() + 1, Niflect::TArrayNif<bool>(pattern.size() + 1, false));
        dp[0][0] = true;

        for (size_t j = 1; j <= pattern.size(); ++j) {
            if (pattern[j - 1] == '*') {
                dp[0][j] = dp[0][j - 1];
            }
        }

        for (size_t i = 1; i <= filename.size(); ++i) {
            for (size_t j = 1; j <= pattern.size(); ++j) {
                if (pattern[j - 1] == filename[i - 1] || pattern[j - 1] == '?') {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else if (pattern[j - 1] == '*') {
                    dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
                }
            }
        }

        return dp[filename.size()][pattern.size()];
}

#ifdef _WIN32
    // 在 Windows 下递归遍历目录
    static void search_directory_windows(const Niflect::CString& directory, const Niflect::CString& pattern, Niflect::TArrayNif<Niflect::CString>& vecFound) {
        Niflect::CString search_path = directory + "/*";
        WIN32_FIND_DATA find_data;
        HANDLE handle = FindFirstFile(search_path.c_str(), &find_data);

        if (handle == INVALID_HANDLE_VALUE) {
            std::cerr << "无法打开目录: " << directory << std::endl;
            return;
        }

        do {
            Niflect::CString filename = find_data.cFileName;
            if (filename == "." || filename == "..") {
                continue; // 跳过 "." 和 ".." 项
            }

            Niflect::CString full_path = directory + "/" + filename;

            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                search_directory_windows(full_path, pattern, vecFound); // 递归搜索子目录
            }
            else {
                if (match(filename, pattern)) {
                    vecFound.push_back(full_path);
                }
            }

        } while (FindNextFile(handle, &find_data) != 0);

        FindClose(handle);
    }
#else
    // 在 Linux 下递归遍历目录
    static void search_directory_linux(const Niflect::CString& directory, const Niflect::CString& pattern, Niflect::TArrayNif<Niflect::CString>& vecFound) {
        DIR* dir = opendir(directory.c_str());
        if (!dir) {
            std::cerr << "无法打开目录: " << directory << std::endl;
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (entry->d_name[0] == '.') {
                continue; // 跳过 "." 和 ".." 项
            }

            Niflect::CString full_path = directory + "/" + entry->d_name;

            if (entry->d_type == DT_DIR) {
                search_directory_linux(full_path, pattern, vecFound); // 递归搜索子目录
            }
            else {
                if (match(entry->d_name, pattern)) {
                    vecFound.push_back(full_path);
                }
            }
        }

        closedir(dir);
    }
#endif

    void SearchFiles(const Niflect::CString& dirPath, const Niflect::CString& pattern, Niflect::TArrayNif<Niflect::CString>& vecFound)
    {
        ASSERT(dirPath.back() == '/');

        //测试参数:
        //F:/Fts/Proj/Test/Interedit/Source/ "*or?.cpp"
        //结果:
        //F:/Fts/Proj/Test/Interedit/Source/\Engine\Test\TestAccessor2.cpp
        //F:/Fts/Proj/Test/Interedit/Source/\Niflect\Memory\Default\DefaultMemory.cpp
        //F:/Fts/Proj/Test/Interedit/Source/\Niflect\Memory\Generic\GenericMemory.cpp
        //F:/Fts/Proj/Test/Interedit/Source/\Niflect\Util\StlCompliant\StlCompliantVector2.cpp
        //F:/Fts/Proj/Test/Interedit/Source/\NiflectGen\Generator\SourceInMemory.cpp

#ifdef _WIN32
        search_directory_windows(dirPath, pattern, vecFound);
#else
        search_directory_linux(dirPath, pattern, vecFound);
#endif
    }
}



#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

namespace NiflectUtil
{
#ifdef _WIN32
    // 在 Windows 下递归遍历目录
    static void CollectFilesRecurs_Windows(const Niflect::CString& rootDirPath, const Niflect::CString& relativeParentDirPath, const CCollectingOption& opt, uint32 depth) {
        auto directory = NiflectUtil::ConcatPath(rootDirPath, relativeParentDirPath);
        Niflect::CString searchPattern = directory;
        if (searchPattern.back() != '/')
            searchPattern += "/*";
        else
            searchPattern += '*';
        WIN32_FIND_DATA find_data;
        HANDLE handle = FindFirstFile(searchPattern.c_str(), &find_data);

        if (handle == INVALID_HANDLE_VALUE) {
            std::cerr << "无法打开目录: " << directory << std::endl;
            return;
        }

        do {
            Niflect::CString filename = find_data.cFileName;
            if (filename == "." || filename == "..") {
                continue; // 跳过 "." 和 ".." 项
            }

            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                if (opt.m_maxDepth == 0 || depth < opt.m_maxDepth)
                {
                    auto relDirPath = NiflectUtil::ConcatPath(relativeParentDirPath, filename);
                    CollectFilesRecurs_Windows(rootDirPath, relDirPath, opt, depth + 1); // 递归搜索子目录
                }
            }
            else {
                SCollectingContext ctx{ relativeParentDirPath, filename };
                opt.m_Func(ctx);
            }

        } while (FindNextFile(handle, &find_data) != 0);

        FindClose(handle);
    }
#else
    // 在 Linux 下递归遍历目录
    static void CollectFilesRecurs_Linux(const Niflect::CString& rootDirPath, const Niflect::CString& relativeParentDirPath, const CCollectingOption& opt, uint32 depth) {
        auto directory = NiflectUtil::ConcatPath(rootDirPath, relativeParentDirPath);
        DIR* dir = opendir(directory.c_str());
        if (!dir) {
            std::cerr << "无法打开目录: " << directory << std::endl;
            return;
        }

        struct dirent* entry;
        while ((entry = readdir(dir))) {
            if (entry->d_name[0] == '.') {
                continue; // 跳过 "." 和 ".." 项
            }

            Niflect::CString filename = entry->d_name;
            if (entry->d_type == DT_DIR) {
                if (opt.m_maxDepth == 0 || depth < opt.m_maxDepth)
                {
                    auto relDirPath = NiflectUtil::ConcatPath(relativeParentDirPath, filename);
                    CollectFilesRecurs_Linux(rootDirPath, relDirPath, opt, depth + 1); // 递归搜索子目录
                }
            }
            else {
                SCollectingContext ctx{ relativeParentDirPath, filename };
                opt.m_Func(ctx);
            }
        }

        closedir(dir);
    }
#endif

    void CollectFiles(const Niflect::CString& dirPath, const CCollectingOption& opt)
    {
        Niflect::CString relativeParentDirPath;
#ifdef _WIN32
        CollectFilesRecurs_Windows(dirPath, relativeParentDirPath, opt, 1);
#else
        CollectFilesRecurs_Linux(dirPath, relativeParentDirPath, opt, 1);
#endif
    }
}











//获取文件时间戳备用代码
// 
//#include <iostream>
//#include <fstream>
//#include <string>
//
//#ifdef _WIN32
//#include <windows.h>
//#include <sys/time.h>
//#else
//#include <sys/stat.h>
//#include <unistd.h>
//#endif
//
//std::string get_file_modification_time(const std::string& filepath) {
//    std::string mod_time_str;
//
//#ifdef _WIN32
//    HANDLE hFile = CreateFile(filepath.c_str(), GENERIC_READ, FILE_SHARE_READ,
//        NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (hFile == INVALID_HANDLE_VALUE) {
//        std::cerr << "Error opening file: " << filepath << std::endl;
//        return "";
//    }
//
//    FILETIME ftLastWriteTime;
//    if (GetFileTime(hFile, NULL, NULL, &ftLastWriteTime)) {
//        SYSTEMTIME st;
//        FileTimeToSystemTime(&ftLastWriteTime, &st);
//        mod_time_str = std::to_string(st.wYear) + "-" +
//            std::to_string(st.wMonth) + "-" +
//            std::to_string(st.wDay) + " " +
//            std::to_string(st.wHour) + ":" +
//            std::to_string(st.wMinute) + ":" +
//            std::to_string(st.wSecond);
//    }
//    else {
//        std::cerr << "Error getting file time." << std::endl;
//    }
//
//    CloseHandle(hFile);
//#else
//    struct stat file_stat;
//    if (stat(filepath.c_str(), &file_stat) == 0) {
//        mod_time_str = std::to_string(file_stat.st_mtime);
//    }
//    else {
//        std::cerr << "Error getting file time." << std::endl;
//        return "";
//    }
//#endif
//
//    return mod_time_str;
//}
//
//int main() {
//    std::string filepath = "example.txt"; // 这里输入您的文件名
//    std::string mod_time = get_file_modification_time(filepath);
//    if (!mod_time.empty()) {
//        std::cout << "Modification time of " << filepath << ": " << mod_time << std::endl;
//    }
//    return 0;
//}