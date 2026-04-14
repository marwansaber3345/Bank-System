# \# 🏦 Bank Management System

# 

# A console-based bank management system built in C++ that supports client management, user management with role-based permissions, and financial transactions.

# 

# ---

# 

# \## 📋 Features

# 

# \- \*\*Client Management\*\* — Add, delete, update, and search clients

# \- \*\*User Management\*\* — Add, delete, update, and search system users

# \- \*\*Transactions\*\* — Deposit, withdraw, and view total balance

# \- \*\*Role-Based Permissions\*\* — Each user has a bitmask of permissions controlling what they can access

# \- \*\*File Persistence\*\* — All data is stored in `.txt` files

# 

# ---

# 

# \## 🗂️ File Structure

# 

# ```

# BankSystem.cpp     # Main source file (all logic in one file)

# Clients.txt        # Auto-generated — stores client records

# Users.txt          # Auto-generated — stores user records

# ```

# 

# ---

# 

# \## 🚀 Getting Started

# 

# \### Requirements

# \- A C++ compiler (g++ recommended)

# \- Windows (uses `system("cls")` and `system("pause>0")`)

# 

# \### Compile

# 

# ```bash

# g++ BankSystem.cpp -o BankSystem

# ```

# 

# \### Run

# 

# ```bash

# ./BankSystem

# ```

# 

# \### First-Time Setup

# 

# The system reads users from `Users.txt`. Create it manually with an admin account before running:

# 

# ```

# admin#//#1234#//#-1

# ```

# 

# > Format: `UserName#//#Password#//#Permissions`

# > Use `-1` for full access.

# 

# ---

# 

# \## 🔐 Permissions System

# 

# Permissions are stored as a \*\*bitmask integer\*\* in `Users.txt`. Each bit represents one permission:

# 

# | Permission     | Bit | Value |

# |----------------|-----|-------|

# | Show Clients   | 0   | 1     |

# | Add Client     | 1   | 2     |

# | Delete Client  | 2   | 4     |

# | Update Client  | 3   | 8     |

# | Find Client    | 4   | 16    |

# | Transactions   | 5   | 32    |

# | Manage Users   | 6   | 64    |

# 

# \*\*Examples:\*\*

# \- `-1` → Full access (Admin)

# \- `127` → All permissions (all bits set: 1+2+4+8+16+32+64)

# \- `33` → Show Clients + Transactions only (1+32)

# 

# ---

# 

# \## 📁 Data Format

# 

# \### Clients.txt

# ```

# A100#//#1234#//#John Smith#//#0501234567#//#5000.000000

# A101#//#5678#//#Jane Doe#//#0509876543#//#12000.000000

# ```

# 

# \### Users.txt

# ```

# admin#//#1234#//#-1

# staff#//#abcd#//#49

# ```

# 

# > Separator used: `#//#`

# 

# ---

# 

# \## 🧭 Navigation

# 

# ```

# Login

# &nbsp;└── Main Menu

# &nbsp;     ├── Show Client List

# &nbsp;     ├── Add New Client

# &nbsp;     ├── Delete Client

# &nbsp;     ├── Update Client Info

# &nbsp;     ├── Find Client

# &nbsp;     ├── Transactions

# &nbsp;     │    ├── Deposit

# &nbsp;     │    ├── Withdraw

# &nbsp;     │    └── Total Balance

# &nbsp;     ├── Manage Users

# &nbsp;     │    ├── Show Users List

# &nbsp;     │    ├── Add New User

# &nbsp;     │    ├── Delete User

# &nbsp;     │    ├── Update User Info

# &nbsp;     │    └── Find User

# &nbsp;     └── Logout

# ```

# 

# ---

# 

# \## 🛠️ Code Structure

# 

# | Section               | Description                                      |

# |-----------------------|--------------------------------------------------|

# | `Structs`             | `sClient`, `sUsers`                              |

# | `Enums`               | Permissions, menu options, transaction options   |

# | `String Utilities`    | `SplitString`                                    |

# | `Record Conversion`   | Convert between structs and file lines           |

# | `File Operations`     | Load, save, and append to `.txt` files           |

# | `Lookup`              | Find clients/users by key                        |

# | `Display`             | Print cards, rows, and tables to console         |

# | `Input Helpers`       | Read and validate user input                     |

# | `Client CRUD`         | Add, delete, update clients                      |

# | `User CRUD`           | Add, delete, update users                        |

# | `Transactions`        | Deposit, withdraw, balance report                |

# | `Navigation`          | GoBack functions for menu flow                   |

# | `Menu Screens`        | Main menu, transaction menu, manage users menu   |

# | `Login`               | Authentication with retry loop                   |

# 

# ---

# 

# \## 📌 Notes

# 

# \- Deleted records are marked with `MarkForDelete = true` and filtered out on save

# \- The system uses \*\*recursive menu navigation\*\* (each screen calls back into its parent)

# \- Passwords are stored in \*\*plain text\*\* — not suitable for production use

# \- Built and tested with \*\*MinGW g++\*\* on Windows

# 

# ---

# 

# \## 👤 Author

# 

# \*\*Marwan Saber\*\*



