#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "raylib.h"

/* INNO SETUP

[Setup]
AppName=My Tool
AppVersion=1.0
DefaultDirName={pf}\MyTool
DefaultGroupName=MyTool
OutputDir=dist
OutputBaseFilename=MyToolSetup

[Files]
Source: "mytool.exe"; DestDir: "{app}"

[Icons]
Name: "{group}\My Tool"; Filename: "{app}\mytool.exe"

*/

////////////////
/// Types

#define bool  _Bool
#define true  1
#define false 0

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef size_t    usize; // Unsigned, pointer-sized
typedef ptrdiff_t isize; // Signed, pointer-sized

typedef float  f32;   // IEEE 754 Single Precision
typedef double f64;   // IEEE 754 Double Precision

////////////////
/// Constants

#define MAX_PATH_LEN 512
#define MAX_FILES    32

////////////////
/// State

typedef struct
{
    char  paths[MAX_FILES][MAX_PATH_LEN];
    char  dirs[MAX_FILES][MAX_PATH_LEN];
    char  outputs[MAX_FILES][MAX_PATH_LEN];

    usize file_count;

} app_state;

////////////////
/// Catch dropped files

static usize catch_dropped_files(char paths[][MAX_PATH_LEN], usize max_files)
{
    if (!IsFileDropped())
        return 0;

    FilePathList files = LoadDroppedFiles();

    usize count = (files.count < max_files) ? files.count : max_files;

    for (usize i = 0; i < count; i++)
    {
        strncpy(paths[i], files.paths[i], MAX_PATH_LEN - 1);
        paths[i][MAX_PATH_LEN - 1] = '\0';
    }

    UnloadDroppedFiles(files);

    return count;
}

////////////////
/// Extract directory safely

static void extract_directory(const char *path, char *dir_out)
{
    const char *dir = GetDirectoryPath(path);

    strncpy(dir_out, dir, MAX_PATH_LEN - 1);
    dir_out[MAX_PATH_LEN - 1] = '\0';
}

////////////////
/// Build output path

static void build_output_path(const char *input, char *out_path)
{
    char dir[MAX_PATH_LEN];
    char name[MAX_PATH_LEN];

    strncpy(dir, GetDirectoryPath(input), MAX_PATH_LEN - 1);
    dir[MAX_PATH_LEN - 1] = '\0';

    strncpy(name, GetFileNameWithoutExt(input), MAX_PATH_LEN - 1);
    name[MAX_PATH_LEN - 1] = '\0';

    snprintf(out_path, MAX_PATH_LEN, "%s/%s_processed.wav", dir, name);
}

////////////////
/// Main

int main(void)
{
    InitWindow(1280, 720, "Drag Files Here");
    SetTargetFPS(60);

    app_state app = {0};

    while (!WindowShouldClose())
    {
        ////////////////////
        // Input

        usize new_files = catch_dropped_files(app.paths, MAX_FILES);

        if (new_files > 0)
        {
            app.file_count = new_files;

            for (usize i = 0; i < app.file_count; i++)
            {
                extract_directory(app.paths[i], app.dirs[i]);
                build_output_path(app.paths[i], app.outputs[i]);
            }
        }

        ////////////////////
        // Render

        BeginDrawing();

        ClearBackground(DARKGRAY);

        DrawText("Drag and drop files", 40, 40, 30, RAYWHITE);

        for (usize i = 0; i < app.file_count; i++)
        {
            int y = 120 + (int)i * 80;

            DrawText("Input:", 40, y, 20, LIGHTGRAY);
            DrawText(app.paths[i], 120, y, 20, GREEN);

            DrawText("Output:", 40, y + 30, 20, LIGHTGRAY);
            DrawText(app.outputs[i], 120, y + 30, 20, SKYBLUE);
        }

        EndDrawing();
    }

    CloseWindow();
}
