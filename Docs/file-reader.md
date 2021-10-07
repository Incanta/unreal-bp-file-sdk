This file covers the nodes available on File Reader instances. To create a File Reader and open a file for reading, use the [`Open File Reader`](./base-nodes#open-file-reader) node.

## Nodes

### File Is Good

Checks to see if the file reader is in a "good" state. This is synonymous to the C++ [`good`](https://www.cplusplus.com/reference/ios/ios/good/) function. This function returns `false` if the file reader is in a bad state, failed, or at the end of the file.

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `false` if the file reader is in a bad state, failed, or at the end of the file. Returns `true` if the file is successfully open and ready to read more bytes (i.e. the pointer is not at the end of the file). |

### Seek File Position

Change the position of the File Reader; this is essentially a pointer of where you're going to start reading for any of the following read nodes. When you open a File Reader, the seek position starts at the `Beginning`. This is synonymous to the C++ [`seekg`](https://www.cplusplus.com/reference/istream/istream/seekg/) function.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Anchor` | [`File SDK File Anchor Enum`](#file-anchor-enum) | **Required** | This defines the _from where_ that `Offset` is applied to. See the [enum description](#file-anchor-enum) for more details. |
| `Offset` | `Integer` | **Required** | Number of bytes to offset from the `Anchor`. |

#### File Anchor Enum

Below are the different enum values for the `Anchor` argument.

| Name | Description |
| - | - |
| `Beginning` | References the beginning of the file. Great for reading metadata, resetting your location, or just going to exact location. If you're in the middle of the file and want to to go to 300 bytes from the beginning, set `Anchor` to `Beginning` and `Offset` to `300`. A negative `Offset` doesn't make sense when using `Beginning`. |
| `Current` | References the current location of the file reader. Great for skipping chunks of data (i.e. your file has an array of stuff and you only want to read the metadata headers for each item, you can read the metadata, skip past the actual chunk, and go to the next metadata location) If you want to advanced your current location 100 bytes backwards, set `Anchor` to `Current` and `Offset` to `-100`. |
| `End` | References the end of the file. Great for reading footers where information is stored at the end of the file. Many times you use this when you open the file, but the metadata you want is near the end of the file a fixed number of bytes. For example, 300 bytes from the end would have `Anchor` set to `End` and `Offset` set to `-300`. A positive `Offset` doesn't make sense when using `Beginning`. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Boolean` | Returns `true` if the file is open and [`Is Good`](#file-is-good); otherwise returns `false`. |

### Read Bytes

Reads a specified number of bytes from the current file reader location as a binary `Byte` array. The file reader location is advanced to where ever it finishes reading.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Num` | `Integer` | **Required** | The number of bytes to read. If `Num` greater than the number of bytes left in the file, it will read to the end of the file. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Integer` | The actual number of bytes read. This is usually only different than `Num` if you reached the end of the file. You should, however, still use [`Is Good`](#file-is-good) to check if you're at the end of the file. |
| `Content` | `Byte` Array | The bytes that were read in as a binary `Byte` array. |

### Read Bytes To End

Reads the rest of the file from the current location as a binary `Byte` array. The file reader location is advanced to the end of the file.

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Integer` | The actual number of bytes read. |
| `Content` | `Byte` Array | The bytes that were read in as a binary `Byte` array. |

### Read String

Reads a specified number of bytes from the current file reader location as a `String`. The file reader location is advanced to where ever it finishes reading.

**Arguments**

| Name | Type | Default Value | Description |
| - | - | - | - |
| `Num` | `Integer` | **Required** | The number of bytes to read. If `Num` greater than the number of bytes left in the file, it will read to the end of the file. |

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Integer` | The actual number of bytes read. This is usually only different than `Num` if you reached the end of the file. You should, however, still use [`Is Good`](#file-is-good) to check if you're at the end of the file. |
| `Content` | `String` | The bytes that were read in as a `String`. |

### Read String To End

Reads the rest of the file from the current location as a `String`. The file reader location is advanced to the end of the file.

**Outputs**

| Name | Type | Description |
| - | - | - |
| `Return Value` | `Integer` | The actual number of bytes read. |
| `Content` | `String` | The bytes that were read in as a `String`. |

### Close File Reader

Closes the file if it's valid/open. Does nothing otherwise.
