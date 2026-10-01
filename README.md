# 🔍 File Tracker (Merkle Tree Vcs)

Hey there! Welcome to **File Tracker**—a high-performance, lightweight directory state tracking tool written entirely in C. 

Have you ever wondered how Git knows exactly which files you've modified without having to literally read every single file line-by-line? The secret sauce is [Merkle Trees](https://en.wikipedia.org/wiki/Merkle_tree), and I built this project to implement that exact concept from scratch! 

---

## 📝 Project Proposal

### Description
**File Tracker** acts as a simplified version control system. It leverages cryptographic hash trees (Merkle trees) to efficiently detect whether files have been added, modified, or deleted within a large and nested directory structure.

### Goals
- **Algorithm Exploration**: Dive deep into tree data structures and recursive algorithms instead of just building another basic CRUD app.
- **Performance**: Avoid blindly re-scanning unmodified directories by reusing hashes based on file metadata.
- **Showcase C Skills**: Implement everything (including custom sorting and recursive file traversal) purely in C without heavy external libraries (just OpenSSL for hashing).

### Specifications
The tool is driven by a straightforward command-line interface with three core commands:
1. `init`: Initializes tracking for a directory by generating the first Merkle tree and saving it.
2. `update`: Re-scans the directory, compares it against the existing state, and updates the index efficiently.
3. `verify`: Compares the live directory against the saved index and precisely prints which files were `Added`, `Modified`, or `Deleted`.

### Design & Architecture
- **Data Structures**: The core is an n-ary tree node (`Node`) representing a file or directory. To guarantee consistent hashing, children are deterministically sorted using a custom Merge Sort.
- **Hashing**: I used OpenSSL's SHA256. Files are hashed directly based on their contents. Directories are hashed by aggregating the hashes of all their children (bottom-up generation).
- **Change Detection**: To detect changes, the `verify` command uses a highly optimized two-pointer recursive comparison on the sorted child arrays. If a directory's hash matches the stored hash, the entire subtree is skipped in O(1) time.

---

## 🛠️ Prerequisites

Before you start, make sure you have the following installed on your system:
- **GCC Compiler** (or any standard C compiler)
- **Make** (for build automation)
- **OpenSSL Development Libraries** 
  - On Ubuntu/Debian: `sudo apt install libssl-dev`
  - On macOS: `brew install openssl`

---

## 🚀 Compilation

I've included a `Makefile` to make building the project as simple as possible. Just clone the repo and run:

```bash
make
```
This will compile all the source files in `src/`, link the headers from `include/`, and generate an executable called `merkleTree`.

*To clean up the build artifacts later, you can run `make clean`.*

---

## 💻 Usage & Examples

Once compiled, you can run the tracker on any directory you want to monitor. Here is a quick example workflow:

### 1. Initialize Tracking
Let's say you have a folder called `my_project`. Initialize it:
```bash
./merkleTree init my_project
```
*This creates a hidden `.ft/index` file that stores the initial Merkle tree state.*

### 2. Verify Changes
Now, try adding a new file, modifying an existing one, or deleting something inside `my_project`. Then run:
```bash
./merkleTree verify my_project
```
**Example Output:**
```
Added: my_project/new_file.txt
Modified: my_project/main.c
Deleted: my_project/old_file.txt
```

### 3. Update the State
Once you are happy with the changes and want to save the new state as the baseline:
```bash
./merkleTree update my_project
```
Now, if you run `verify` again, it will show no changes!

---

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. Feel free to fork it, learn from it, and modify it!