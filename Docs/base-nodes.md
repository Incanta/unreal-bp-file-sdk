This file covers all of the base nodes as part of the exported Blueprint Library. These are all nodes you can call anywhere without any context.

## Paths Are Absolute

All paths to files are **absolute** (as opposed to relative). This means that all arguments asking for a file/directory name need to be the exact, absolute location.

For example, here are some examples (not recommendations of where to put your data) of what an absolute path looks like:
- Windows: `C:\Users\<user>\AppData\Local\Project\project.csv`
- Mac: `/Users/<user>/Library/Preferences/Project/project.csv`
- Linux: `/home/<user>/.config/Project/project.csv`

These are incorrect paths to provide:
- `project.csv`
- `Content/project.csv`

Checkout our TODO examples of ways to help you get absolute paths to various popular folders.

## Nodes

### Open File Reader

Creates a [File Reader](./file-reader) instance; File Readers can be used to control how the file is read. You can seek to different parts of the file, only read in a specified number of bytes, etc. This gives you the most amount of control and is great for only reading metadata in large files.

**Be sure to call the [`Close`](./file-reader#close-file-reader) node when you're done with the file!**

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) of the file that you want to open for reading. |
| `Open In Binary Mode` | `Boolean` | `false` | Setting this to `true` will open the file for reading binary data; `false` opens the file to read text data. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | [`File SDK File Reader`](./file-reader) | The [File Reader](./file-reader) instance regardless of the status of successful open. See the [`Is Good`](./file-reader#file-is-good) node before attempting to read anything. |

### Create File

Creates an empty file. Do note that write [string](#write-string-to-file)/[bytes](#write-bytes-to-file) to file will create the file if it doesn't exist.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to create. |
| `Clear Contents If Exists` | `Boolean` | `true` | If set to `true` this node **will completely delete** any content in the file. |
| `Create Directory Tree` | `Boolean` | `true` | If set to `true`, any missing folders to get to `File Name` will be automatically created. For example, if you provided `/path/to/project/file.txt` and `/path` exists but `/path/to` does not exist, setting this to true will create `/path/to` and `/path/to/project` directories. If this is set to `false` and the directories don't exist, the file will not be created. An error will not be thrown. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file was successfully deleted. Returns `false` if the file doesn't exist or the file couldn't be deleted. |

### Delete File

Deletes a file if it exists; does nothing if it doesn't exist.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to delete. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file was successfully deleted. Returns `false` if the file doesn't exist or the file couldn't be deleted. |

### Create Directory

Creates a directory.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Directory Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory you want to create. |
| `Create Directory Tree` | `Boolean` | `true` | If set to `true`, any missing folders to get to `File Name` will be automatically created. For example, if you provided `/path/to/project/file.txt` and `/path` exists but `/path/to` does not exist, setting this to true will create `/path/to` and `/path/to/project` directories. If this is set to `false` and the directories don't exist, the file will not be created. An error will not be thrown. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns whether or not the directory was successfully created. |

### Delete Directory

Deletes a directory.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Directory Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory you want to delete. |
| `Recursive` | `Boolean` | `true` | If set to `true`, all files and folders in this directory will be deleted. If set to `false`, the directory will only be deleted if it is empty. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns whether or not the directory was successfully deleted. |

### Rename File or Directory

Renames/moves a file or a directory to a new name/location.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Source` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file/directory that you want to rename/move. |
| `Destination` | `String` | **Required** | An [absolute path](#paths-are-absolute) of the new location/name you want for the file or directory. You cannot provide `/path/to/file.csv` for `Source` and only provide `file-old.csv` for `Destination`; you must provide the full absolute path of the new file (i.e. `/path/to/file-old.csv`). |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns whether or not the file/directory was successfully renamed/moved. |

### Copy File

Copies a file, creating a duplicate version. Performs the copy operation on the main game thread; this can have performance issues for large files. See [Copy File Async](#copy-file-async) to perform the copy in the background (in a separate thread).

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Source` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to copy. |
| `Destination` | `String` | **Required** | An [absolute path](#paths-are-absolute) to where you want to copy the file to. You cannot provide `/path/to/file.csv` for `Source` and only provide `file-old.csv` for `Destination`; you must provide the full absolute path of the new file (i.e. `/path/to/file-old.csv`). |
| `Progress Callback` | [`Copy Progress Event`](#copy-progress-event) | | This allows you to attach an event to receive execution when progress is made, allowing you to update a UI or other variables about the progress left for copying the file. |
| `Chunk Size In Kilobytes` | `Integer` | `1024` | The number of kilobytes you want each chunk of data to be copied as. Smaller numbers can give you more fine progress updates, but at the cost of more disk IO operations, potentially slowing down the overall copy. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `false` if `Source` could not be opened (likely because it doesn't exist) or if `Destination` could not be opened for write permissions (likely the directory doesn't exist, or the user doesn't have permissions to write there). Returns `true` if the copy was successful. |

### Copy File Async

Same as [Copy File](#copy-file), but performs the copy operation in a background thread, preventing the game thread from waiting for the copy operation to finish.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Source` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to copy. |
| `Destination` | `String` | **Required** | An [absolute path](#paths-are-absolute) to where you want to copy the file to. You cannot provide `/path/to/file.csv` for `Source` and only provide `file-old.csv` for `Destination`; you must provide the full absolute path of the new file (i.e. `/path/to/file-old.csv`). |
| `Progress Callback` | [`Copy Progress Event`](#copy-progress-event) | | This allows you to attach an event to receive execution when progress is made, allowing you to update a UI or other variables about the progress left for copying the file. |
| `Chunk Size In Kilobytes` | `Integer` | `1024` | The number of kilobytes you want each chunk of data to be copied as. Smaller numbers can give you more fine progress updates, but at the cost of more disk IO operations, potentially slowing down the overall copy. |

**Outputs**

You must listen to the `Progress Callback` to determine if the copy finished successfully (when it reaches 100%); unfortunately there's no way to detect failure with this node currently. Eventually, we would like to implement this as a latent node that gives you `Exec` pins on on success and on failure.

### Copy Directory

Copies a directory, including all of its contents, creating a duplicate version. Performs the copy operation on the main game thread; this can have performance issues for large directories. See [Copy Directory Async](#copy-directory-async) to perform the copy in the background (in a separate thread).

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Source` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory you want to copy. |
| `Destination` | `String` | **Required** | An [absolute path](#paths-are-absolute) to where you want to copy the directory to. |
| `Progress Callback` | [`Copy Progress Event`](#copy-progress-event) | | This allows you to attach an event to receive execution when progress is made, allowing you to update a UI or other variables about the progress left for copying the directory. |
| `Overwrite Destination` | `Boolean` | `false` | If set to `true`, if a particular file exists in the respective `Destination` location, it will be overwritten with the new contents. Otherwise it will be ignored. For example if you're copying `/path/from` to `/path/to`, and `/path/to/file.txt` already exists, if this is set to `true`, `/path/from/file.txt` will overwrite `/path/to/file.txt`, if set to `false`, `/path/to/file.txt` will remain unchanged. |
| `Chunk Size In Kilobytes` | `Integer` | `1024` | The number of kilobytes you want each chunk of data to be copied as. Smaller numbers can give you more fine progress updates, but at the cost of more disk IO operations, potentially slowing down the overall copy. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the copy finished successfully. Returns `false` if any of the files couldn't be copied or if subdirectories couldn't be created (usually this happens if there are permissions issues or files are open in other programs). |

### Copy Directory Async

Same as [Copy Directory](#copy-directory), but performs the copy operation in a background thread, preventing the game thread from waiting for the copy operation to finish.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Source` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory you want to copy. |
| `Destination` | `String` | **Required** | An [absolute path](#paths-are-absolute) to where you want to copy the directory to. |
| `Progress Callback` | [`Copy Progress Event`](#copy-progress-event) | | This allows you to attach an event to receive execution when progress is made, allowing you to update a UI or other variables about the progress left for copying the directory. |
| `Overwrite Destination` | `Boolean` | `false` | If set to `true`, if a particular file exists in the respective `Destination` location, it will be overwritten with the new contents. Otherwise it will be ignored. For example if you're copying `/path/from` to `/path/to`, and `/path/to/file.txt` already exists, if this is set to `true`, `/path/from/file.txt` will overwrite `/path/to/file.txt`, if set to `false`, `/path/to/file.txt` will remain unchanged. |
| `Chunk Size In Kilobytes` | `Integer` | `1024` | The number of kilobytes you want each chunk of data to be copied as. Smaller numbers can give you more fine progress updates, but at the cost of more disk IO operations, potentially slowing down the overall copy. |

**Outputs**

You must listen to the `Progress Callback` to determine if the copy finished successfully (when it reaches 100%); unfortunately there's no way to detect failure with this node currently. Eventually, we would like to implement this as a latent node that gives you `Exec` pins on on success and on failure.

### Copy Progress Event

This is an event that you can bind to to any of the copy operations listed above to get updates of the copy progress.

**Arguments**

| Name | Type | Description |
| - | - | - |
| `Kilobytes Written` | `Integer` | Number of how many kilobytes have been written. |
| `Total Kilobytes` | `Integer` | Number of the total kilobytes for the entire operation. This is **not** "Kilobytes Left", and should not change for an operation. |

### Read String from File

Reads the entire contents of a file as text into a string.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you would like to read. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file could be opened and read successfully. |
| `Content` | `String` | The entire contents of `File Name`. |

### Write String to File

Writes a string as text to a file, with options to overwrite/append as well as encoding options. Will create the file if it doesn't exist. This node **expects** the parent directory to already exist (i.e. it will not create directories for you, and will return `false` if the parent directories do not exist).

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you would like to write. |
| `Content` | `String` | **Required** | The text you would like to write to the file. |
| `Append` | `Boolean` | `false` | If set to `true`, the file will be erased before writing `Content` to it; if set to `false`, `Content` will be written at the end of hte file. New lines/carriage returns will **not** be automatically inserted for you. |
| `Encoding` | `Encoding Options Enum` | `Auto Detect` | Encoding options that are passed down to the file writer. `Auto Detect` will do its best to determine the encoding of the string, but you can also use one of the `Force (Ansi | Unicode | UTF8 | UTF8WithoutBOM)` to force how the contents are encoded. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file could be opened and written successfully. |

### Write Bytes to File

Writes a `Byte` array as binary data to a file. This node currently **always** overwrites the file contents. If you have a use case for appending binary files, please [let us know](/plugins/support). Will create the file if it doesn't exist. This node **expects** the parent directory to already exist (i.e. it will not create directories for you, and will return `false` if the parent directories do not exist).

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to write. |
| `Content` | `Byte` Array | **Required** | The bytes you would like to write to the file. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file was opened wnd written successfully. |

### Read Bytes from File

Reads the entire contents of a file as a binary `Byte` array.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `File Name` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file you want to read. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file was opened and read successfully. |
| `Content` | `Byte` Array | The entire contents of `File Name`. |

### Get Files or Directories from Directory

Searches for files or directories in a directory.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Directory to Search` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory that you want to search in. |
| `Filter Files With Extension` | `String` | | Only used if `File Type` is `File`. This argument allows you to filter the results by the file extension. Proper usage of this argument is `.txt`, `.csv`, etc, including the `.`. |
| `Search Subfolders` | `Boolean` | `false` | Whether or not to search subdirectories (`true`) or just search the single folder provided (`false`). |
| `File Type` | `File Type Enum` | `File`| Whether or not to search for a `File` or a `Directory`. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `String` Array | An array of [absolute paths](#paths-are-absolute) to each file or directory found. |

### Get Directory Contents with File Info

Lists the contents of a directory, including both files and directories, with supplemental information.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Directory` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the directory that you want to list contents of. |
| `Search Subfolders` | `Boolean` | `false` | Whether or not to search subdirectories (`true`) or just search the single folder provided (`false`). |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | [`File Info Struct`](#file-info-struct) Array | An array of structs, one for each file and directory, each containing detailed information of the file/directory. |

### Get File or Directory Info

Returns detailed information for a specific file or directory.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Path` | `String` | **Required** | An [absolute path](#paths-are-absolute) to the file or directory you want information for. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Info` | [`File Info Struct`](#file-info-struct) | The detailed info struct of the file. |

### File Info Struct

This is a struct that has detailed information for a specific file or directory.

| Name | Type | Description |
| - | - | - |
| `Absolute Path` | `String` | The absolute path (including the file/directory name) of the file/directory. |
| `Filename` | `String` | Only the file/directory name without the preceding path/location. |
| `Creation Time` | `Date Time` | The time that the file or directory was originally created, or `FDateTime::MinValue` if the creation time is unknown. |
| `Access Time` | `Date Time` | The time that the file or directory was last accessed, or FDateTime::MinValue if the access time is unknown. |
| `Modification Time` | `Date Time` | The time the the file or directory was last modified, or FDateTime::MinValue if the modification time is unknown. |
| `File Size` | `Integer64` | Size of the file (in bytes), or -1 if the file size is unknown. |
| `Is Directory` | `Boolean` | `true` if this is a directory, `false` if it is a file. |
| `Is Read Only` | `Boolean` | `true` if this file/directory is read-only. |
| `Is Valid` | `Boolean` | `true` if file or directory was found, `false` otherwise. Note that this value being `true` does not ensure that the other members are filled in with meaningful data, as not all file systems have access to all of this data. |

### Get Current Username

Retrieves the current username for whoever is running the program/game. It's `<user>` in the below examples:
- Windows: `C:\Users\<user>`
- Mac: `/Users/<user>`
- Linux: `/home/<user>`

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `String` | On Windows, this returns the environment variable `%USERNAME%`. On Mac and Linux, this returns the environment variable `$USER`. On other platforms, it returns `PLATFORM_NOT_SUPPORTED`. |

### Get Current User Home Directory

Returns the current user's home directory. It's the whole path in the below examples:
- Windows: `C:\Users\<user>`
- Mac: `/Users/<user>`
- Linux: `/home/<user>`

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `String` | On Windows, this returns the environment variable `%HOMEDRIVE%` concatenated with `%HOMEPATH%` (i.e. `%HOMEDRIVE%%HOMEPATH%`). On Mac and Linux, this returns the environment variable `$HOME`. On other platforms, it returns `PLATFORM_NOT_SUPPORTED`. |

### Get Environment Variable

Returns the value of an environment variable.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Variable Name` | `String` | **Required** | On Windows, you fetch environment variables in Command Prompt with something like `%VAR%`; on Mac and Linux, you fetch environment variables in the terminal with something like `$VAR`. In both cases, `Variable Name` is `VAR` without the `%` or `$`. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `String` | Returns the value of the environment variable if it's set. If it's not set it returns an empty string. On target platforms that are not Windows, Mac, or Linux, `PLATFORM_NOT_SUPPORTED` is returned. |
