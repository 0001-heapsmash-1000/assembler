/* x86 assembler
 * the-scientist@rootstorm.com
 * spl0its-r-us security
 */

#include <stdlib.h> // int 8 
#include "./include/assembler_sysmov.h"
#include "./include/assembler_perfect_hash.h"

 /* syscalls:
  * Parameter #	Register
  * Parameter 1	EBX
  * Parameter 2	ECX
  * Parameter 3	EDX
  * Parameter 4	ESI
  * Parameter 5	EDI
  * Parameter 6	EBP
  */

struct {
  int   eax_value;
  char* sys_name;
} syscall_value_table[] = {
  { 0  ,   "sys_restart_syscall"         },
  { 1  ,   "sys_exit"                    },
  { 2  ,   "sys_fork"                    },
  { 3  ,   "sys_read"                    },
  { 4  ,   "sys_write"                   },
  { 5  ,   "sys_open"                    },
  { 6  ,   "sys_close"                   },
  { 7  ,   "sys_waitpid"                 },
  { 8  ,   "sys_creat"                   },
  { 9  ,   "sys_link"                    },
  { 10 ,   "sys_unlink"                  },
  { 11 ,   "sys_execve"                  },
  { 12 ,   "sys_chdir"                   },
  { 13 ,   "sys_time32"                  },
  { 14 ,   "sys_mknod"                   },
  { 15 ,   "sys_chmod"                   },
  { 16 ,   "sys_lchown16"                },
  { 17 ,   "sys_break"                   },
  { 18 ,   "sys_stat"                    },
  { 19 ,   "sys_lseek"                   },
  { 20 ,   "sys_getpid"                  },
  { 21 ,   "sys_mount"                   },
  { 22 ,   "sys_umount"                  },
  { 33 ,   "sys_access"                  },
  { 34 ,   "sys_nice"                    },
  { 35 ,   "sys_ftime"                   },
  { 36 ,   "sys_sync"                    },
  { 37 ,   "sys_kill"                    },
  { 38 ,   "sys_rename"                  },
  { 39 ,   "sys_mkdir"                   },
  { 40 ,   "sys_rmdir"                   },
  { 41 ,   "sys_dup"                     },
  { 42 ,   "sys_pipe"                    },
  { 43 ,   "sys_times"                   },
  { 44 ,   "sys_prof"                    },
  { 45 ,   "sys_brk"                     },
  { 46 ,   "sys_setgid16"                },
  { 47 ,   "sys_getgid16"                },
  { 48 ,   "sys_signal"                  },
  { 49 ,   "sys_geteuid16"               },
  { 50 ,   "sys_getegid16"               },
  { 51 ,   "sys_acct"                    },
  { 52 ,   "sys_umount"                  },
  { 53 ,   "sys_lock"                    },
  { 54 ,   "sys_ioctl"                   },
  { 55 ,   "sys_fcntl"                   },
  { 56 ,   "sys_mpx"                     },
  { 57 ,   "sys_setpgid"                 },
  { 58 ,   "sys_ulimit"                  },
  { 59 ,   "sys_olduname"                },
  { 60 ,   "sys_umask"                   },
  { 61 ,   "sys_chroot"                  },
  { 62 ,   "sys_ustat"                   },
  { 63 ,   "sys_dup2"                    },
  { 73 ,   "sys_sigpending"              },
  { 74 ,   "sys_sethostname"             },
  { 75 ,   "sys_setrlimit"               },
  { 76 ,   "sys_old_getrlimit"           },
  { 77 ,   "sys_getrusage"               },
  { 78 ,   "sys_gettimeofday"            },
  { 79 ,   "sys_settimeofday"            },
  { 80 ,   "sys_getgroups16"             },
  { 81 ,   "sys_setgroups16"             },
  { 82 ,   "sys_old_select"              },
  { 83 ,   "sys_symlink"                 },
  { 84 ,   "sys_lstat"                   },
  { 85 ,   "sys_readlink"                },
  { 86 ,   "sys_uselib"                  },
  { 87 ,   "sys_swapon"                  },
  { 88 ,   "sys_reboot"                  },
  { 89 ,   "sys_old_readdir"             },
  { 90 ,   "sys_old_mmap"                },
  { 91 ,   "sys_munmap"                  },
  { 92 ,   "sys_truncate"                },
  { 93 ,   "sys_ftruncate"               },
  { 94 ,   "sys_fchmod"                  },
  { 95 ,   "sys_fchown16"                },
  { 96 ,   "sys_getpriority"             },
  { 97 ,   "sys_setpriority"             },
  { 98 ,   "sys_profil"                  },
  { 99 ,   "sys_statfs"                  },
  { 100 ,  "sys_fstatfs"                 },
  { 101 ,  "sys_ioperm"                  },
  { 102 ,  "sys_socketcall"              },
  { 103 ,  "sys_syslog"                  },
  { 104 ,  "sys_setitimer"               },
  { 105 ,  "sys_getitimer"               },
  { 106 ,  "sys_newstat"                 },
  { 107 ,  "sys_newlstat"                },
  { 108 ,  "sys_newfstat"                },
  { 109 ,  "sys_uname"                   },
  { 110 ,  "sys_iopl"                    },
  { 113 ,  "sys_vm86old"                 },
  { 114 ,  "sys_wait4"                   },
  { 115 ,  "sys_swapoff"                 },
  { 116 ,  "sys_sysinfo"                 },
  { 117 ,  "sys_ipc"                     },
  { 118 ,  "sys_fsync"                   },
  { 119 ,  "sys_sigreturn"               },
  { 120 ,  "sys_clone"                   },
  { 121 ,  "sys_setdomainname"           },
  { 122 ,  "sys_newuname"                },
  { 123 ,  "sys_modify_ldt"              },
  { 124 ,  "sys_adjtimex_time32"         },
  { 125 ,  "sys_mprotect"                },
  { 126 ,  "sys_sigprocmask"             },
  { 127 ,  "sys_create_module"           },
  { 128 ,  "sys_init_module"             },
  { 129 ,  "sys_delete_module"           },
  { 130 ,  "sys_get_kernel_syms"         },
  { 131 ,  "sys_quotactl"                },
  { 132 ,  "sys_getpgid"                 },
  { 133 ,  "sys_fchdir"                  },
  { 134 ,  "sys_ni_syscall"              },
  { 135 ,  "sys_sysfs"                   },
  { 136 ,  "sys_personality"             },
  { 137 ,  "sys_afs_syscall"             },
  { 138 ,  "sys_setfsuid16"              },
  { 139 ,  "sys_setfsgid16"              },
  { 140 ,  "sys_llseek"                  },
  { 141 ,  "sys_getdents"                },
  { 142 ,  "sys_select"                  },
  { 143 ,  "sys_flock"                   },
  { 144 ,  "sys_msync"                   },
  { 145 ,  "sys_readv"                   },
  { 146 ,  "sys_writev"                  },
  { 147 ,  "sys_getsid"                  },
  { 148 ,  "sys_fdatasync"               },
  { 149 ,  "sys_ni_syscall"              },
  { 150 ,  "sys_mlock"                   },
  { 151 ,  "sys_munlock"                 },
  { 152 ,  "sys_mlockall"                },
  { 153 ,  "sys_munlockall"              },
  { 154 ,  "sys_sched_setparam"          },
  { 155 ,  "sys_sched_getparam"          },
  { 156 ,  "sys_sched_setscheduler"      },
  { 157 ,  "sys_sched_getscheduler"      },
  { 158 ,  "sys_sched_yield"             },
  { 159 ,  "sys_sched_get_priority_max"  },
  { 160 ,  "sys_sched_get_priority_min"  },
  { 161 ,  "sys_sched_rr_get_interval_t" },
  { 162 ,  "sys_nanosleep_time32"        },
  { 163 ,  "sys_mremap"                  },
  { 164 ,  "sys_setresuid16"             },
  { 165 ,  "sys_getresuid16"             },
  { 166 ,  "sys_vm86"                    },
  { 167 ,  "sys_query_module"            },
  { 168 ,  "sys_poll"                    },
  { 169 ,  "sys_nfsservctl"              },
  { 170 ,  "sys_setresgid16"             },
  { 171 ,  "sys_getresgid16"             },
  { 172 ,  "sys_prctl"                   },
  { 173 ,  "sys_rt_sigreturn"            },
  { 174 ,  "sys_rt_sigaction"            },
  { 175 ,  "sys_rt_sigprocmask"          },
  { 176 ,  "sys_rt_sigpending"           },
  { 177 ,  "sys_rt_sigtimedwait_time32"  },
  { 178 ,  "sys_rt_sigqueueinfo"         },
  { 179 ,  "sys_rt_sigsuspend"           },
  { 180 ,  "sys_ia32_pread64"            },
  { 181 ,  "sys_ia32_pwrite64"           },
  { 182 ,  "sys_chown16"                 },
  { 183 ,  "sys_getcwd"                  },
  { 184 ,  "sys_capget"                  },
  { 185 ,  "sys_capset"                  },
  { 186 ,  "sys_sigaltstack"             },
  { 187 ,  "sys_sendfile"                },
  { 188 ,  "sys_getpmsg"                 },
  { 189 ,  "sys_putpmsg"                 },
  { 190 ,  "sys_vfork"                   },
  { 191 ,  "sys_getrlimit"               },
  { 192 ,  "sys_mmap_pgoff"              },
  { 193 ,  "sys_ia32_truncate64"         },
  { 194 ,  "sys_ia32_ftruncate64"        },
  { 195 ,  "sys_stat64"                  },
  { 196 ,  "sys_lstat64"                 },
  { 197 ,  "sys_fstat64"                 },
  { 198 ,  "sys_lchown"                  },
  { 199 ,  "sys_getuid"                  },
  { 200 ,  "sys_getgid"                  },
  { 201 ,  "sys_geteuid"                 },
  { 202 ,  "sys_getegid"                 },
  { 203 ,  "sys_setreuid"                },
  { 204 ,  "sys_setregid"                },
  { 205 ,  "sys_getgroups"               },
  { 206 ,  "sys_setgroups"               },
  { 207 ,  "sys_fchown"                  },
  { 208 ,  "sys_setresuid"               },
  { 209 ,  "sys_getresuid"               },
  { 210 ,  "sys_setresgid"               },
  { 211 ,  "sys_getresgid"               },
  { 212 ,  "sys_chown"                   },
  { 213 ,  "sys_setuid"                  },
  { 214 ,  "sys_setgid"                  },
  { 215 ,  "sys_setfsuid"                },
  { 216 ,  "sys_setfsgid"                },
  { 217 ,  "sys_pivot_root"              },
  { 218 ,  "sys_mincore"                 },
  { 219 ,  "sys_madvise"                 },
  { 220 ,  "sys_getdents64"              },
  { 221 ,  "sys_fcntl64"                 },
  { 224 ,  "sys_gettid"                  },
  { 225 ,  "sys_ia32_readahead"          },
  { 226 ,  "sys_setxattr"                },
  { 227 ,  "sys_lsetxattr"               },
  { 228 ,  "sys_fsetxattr"               },
  { 229 ,  "sys_getxattr"                },
  { 230 ,  "sys_lgetxattr"               },
  { 231 ,  "sys_fgetxattr"               },
  { 232 ,  "sys_listxattr"               },
  { 233 ,  "sys_llistxattr"              },
  { 234 ,  "sys_flistxattr"              },
  { 235 ,  "sys_removexattr"             },
  { 236 ,  "sys_lremovexattr"            },
  { 237 ,  "sys_fremovexattr"            },
  { 238 ,  "sys_tkill"                   },
  { 239 ,  "sys_sendfile64"              },
  { 240 ,  "sys_futex_time32"            },
  { 241 ,  "sys_sched_setaffinity"       },
  { 242 ,  "sys_sched_getaffinity"       },
  { 243 ,  "sys_set_thread_area"         },
  { 244 ,  "sys_get_thread_area"         },
  { 245 ,  "sys_io_setup"                },
  { 246 ,  "sys_io_destroy"              },
  { 247 ,  "sys_io_getevents_time32"     },
  { 248 ,  "sys_io_submit"               },
  { 249 ,  "sys_io_cancel"               },
  { 250 ,  "sys_ia32_fadvise64"          },
  { 252 ,  "sys_exit_group"              },
  { 253 ,  "sys_lookup_dcookie"          },
  { 254 ,  "sys_epoll_create"            },
  { 255 ,  "sys_epoll_ctl"               },
  { 256 ,  "sys_epoll_wait"              },
  { 257 ,  "sys_remap_file_pages"        },
  { 258 ,  "sys_set_tid_address"         },
  { 259 ,  "sys_timer_create"            },
  { 260 ,  "sys_timer_settime32"         },
  { 261 ,  "sys_timer_gettime32"         },
  { 262 ,  "sys_timer_getoverrun"        },
  { 263 ,  "sys_timer_delete"            },
  { 264 ,  "sys_clock_settime32"         },
  { 265 ,  "sys_clock_gettime32"         },
  { 266 ,  "sys_clock_getres_time32"     },
  { 267 ,  "sys_clock_nanosleep_time32"  },
  { 268 ,  "sys_statfs64"                },
  { 269 ,  "sys_fstatfs64"               },
  { 270 ,  "sys_tgkill"                  },
  { 271 ,  "sys_utimes_time32"           },
  { 272 ,  "sys_ia32_fadvise64_64"       },
  { 273 ,  "sys_vserver"                 },
  { 274 ,  "sys_mbind"                   },
  { 275 ,  "sys_get_mempolicy"           },
  { 276 ,  "sys_set_mempolicy"           },
  { 277 ,  "sys_mq_open"                 },
  { 278 ,  "sys_mq_unlink"               },
  { 279 ,  "sys_mq_timedsend_time32"     },
  { 280 ,  "sys_mq_timedreceive_time32"  },
  { 281 ,  "sys_mq_notify"               },
  { 282 ,  "sys_mq_getsetattr"           },
  { 283 ,  "sys_kexec_load"              },
  { 284 ,  "sys_waitid"                  },
  { 286 ,  "sys_add_key"                 },
  { 287 ,  "sys_request_key"             },
  { 288 ,  "sys_keyctl"                  },
  { 289 ,  "sys_ioprio_set"              },
  { 290 ,  "sys_ioprio_get"              },
  { 291 ,  "sys_inotify_init"            },
  { 292 ,  "sys_inotify_add_watch"       },
  { 293 ,  "sys_inotify_rm_watch"        },
  { 294 ,  "sys_migrate_pages"           },
  { 295 ,  "sys_openat"                  },
  { 296 ,  "sys_mkdirat"                 },
  { 297 ,  "sys_mknodat"                 },
  { 298 ,  "sys_fchownat"                },
  { 299 ,  "sys_futimesat_time32"        },
  { 300 ,  "sys_fstatat64"               },
  { 301 ,  "sys_unlinkat"                },
  { 302 ,  "sys_renameat"                },
  { 303 ,  "sys_linkat"                  },
  { 304 ,  "sys_symlinkat"               },
  { 305 ,  "sys_readlinkat"              },
  { 306 ,  "sys_fchmodat"                },
  { 307 ,  "sys_faccessat"               },
  { 308 ,  "sys_pselect6_time32"         },
  { 309 ,  "sys_ppoll_time32"            },
  { 310 ,  "sys_unshare"                 },
  { 311 ,  "sys_set_robust_list"         },
  { 312 ,  "sys_get_robust_list"         },
  { 313 ,  "sys_splice"                  },
  { 314 ,  "sys_ia32_sync_file_range"    },
  { 315 ,  "sys_tee"                     },
  { 316 ,  "sys_vmsplice"                },
  { 317 ,  "sys_move_pages"              },
  { 318 ,  "sys_getcpu"                  },
  { 319 ,  "sys_epoll_pwait"             },
  { 320 ,  "sys_utimensat_time32"        },
  { 321 ,  "sys_signalfd"                },
  { 322 ,  "sys_timerfd_create"          },
  { 323 ,  "sys_eventfd"                 },
  { 324 ,  "sys_ia32_fallocate"          },
  { 325 ,  "sys_timerfd_settime32"       },
  { 326 ,  "sys_timerfd_gettime32"       },
  { 327 ,  "sys_signalfd4"               },
  { 328 ,  "sys_eventfd2"                },
  { 329 ,  "sys_epoll_create1"           },
  { 330 ,  "sys_dup3"                    },
  { 331 ,  "sys_pipe2"                   },
  { 332 ,  "sys_inotify_init1"           },
  { 333 ,  "sys_preadv"                  },
  { 334 ,  "sys_pwritev"                 },
  { 335 ,  "sys_rt_tgsigqueueinfo"       },
  { 336 ,  "sys_perf_event_open"         },
  { 337 ,  "sys_recvmmsg_time32"         },
  { 338 ,  "sys_fanotify_init"           },
  { 339 ,  "sys_fanotify_mark"           },
  { 340 ,  "sys_prlimit64"               },
  { 341 ,  "sys_name_to_handle_at"       },
  { 343 ,  "sys_clock_adjtime32"         },
  { 344 ,  "sys_syncfsc"                 },
  { 347 ,  "sys_process_vm_readv"        },
  { 348 ,  "sys_process_vm_writev"       },
  { 349 ,  "sys_kcmp"                    },
  { 350 ,  "sys_finit_module"            },
  { 351 ,  "sys_sched_setattr"           },
  { 352 ,  "sys_sched_getattr"           },
  { 353 ,  "sys_renameat2"               },
  { 354 ,  "sys_seccomp"                 },
  { 355 ,  "sys_getrandom"               },
  { 356 ,  "sys_memfd_create"            },
  { 357 ,  "sys_bpf"                     },
  { 358 ,  "sys_execveat"                },
  { 359 ,  "sys_socket"                  },
  { 360 ,  "sys_socketpair"              },
  { 361 ,  "sys_bind"                    },
  { 362 ,  "sys_connect"                 },
  { 363 ,  "sys_listen"                  },
  { 364 ,  "sys_accept4"                 },
  { 365 ,  "sys_getsockopt"              },
  { 366 ,  "sys_setsockopt"              },
  { 367 ,  "sys_getsockname"             },
  { 368 ,  "sys_getpeername"             },
  { 369 ,  "sys_sendto"                  },
  { 370 ,  "sys_sendmsg"                 },
  { 371 ,  "sys_recvfrom"                },
  { 372 ,  "sys_recvmsg"                 },
  { 373 ,  "sys_shutdown"                },
  { 374 ,  "sys_userfaultfd"             },
  { 375 ,  "sys_membarrier"              },
  { 376 ,  "sys_mlock2"                  },
  { 377 ,  "sys_copy_file_range"         },
  { 378 ,  "sys_preadv2"                 },
  { 379 ,  "sys_pwritev2"                },
  { 380 ,  "sys_pkey_mprotect"           },
  { 381 ,  "sys_pkey_alloc"              },
  { 382 ,  "sys_pkey_free"               },
  { 383 ,  "sys_statx"                   },
  { 384 ,  "sys_arch_prctl"              },
  { 385 ,  "sys_io_pgetevents_time32"    },
  { 386 ,  "sys_rseq"                    },
  { 393 ,  "sys_semget"                  },
  { 394 ,  "sys_semctl"                  },
  { 395 ,  "sys_shmget"                  },
  { 396 ,  "sys_shmctl"                  },
  { 397 ,  "sys_shmat"                   },
  { 398 ,  "sys_shmdt"                   },
  { 399 ,  "sys_msgget"                  },
  { 400 ,  "sys_msgsnd"                  },
  { 401 ,  "sys_msgrcv"                  },
  { 402 ,  "sys_msgctl"                  },
  { 403 ,  "sys_clock_gettime"           },
  { 404 ,  "sys_clock_settime"           },
  { 405 ,  "sys_clock_adjtime"           },
  { 406 ,  "sys_clock_getres"            },
  { 407 ,  "sys_clock_nanosleep"         },
  { 408 ,  "sys_timer_gettime"           },
  { 409 ,  "sys_timer_settime"           },
  { 410 ,  "sys_timerfd_gettime"         },
  { 411 ,  "sys_timerfd_settime"         },
  { 412 ,  "sys_utimensat"               },
  { 413 ,  "sys_pselect6"                },
  { 414 ,  "sys_ppoll"                   },
  { 416 ,  "sys_io_pgetevents"           },
  { 417 ,  "sys_recvmmsg"                },
  { 418 ,  "sys_mq_timedsend"            },
  { 419 ,  "sys_mq_timedreceive"         },
  { 420 ,  "sys_semtimedop"              },
  { 421 ,  "sys_rt_sigtimedwait"         },
  { 422 ,  "sys_futex"                   },
  { 423 ,  "sys_sched_rr_get_interval"   },
  { 424 ,  "sys_pidfd_send_signal"       },
  { 425 ,  "sys_io_uring_setup"          },
  { 426 ,  "sys_io_uring_enter"          },
  { 427 ,  "sys_io_uring_register"       },
  { 428 ,  "sys_open_tree"               },
  { 429 ,  "sys_move_mount"              },
  { 430 ,  "sys_fsopen"                  },
  { 431 ,  "sys_fsconfig"                },
  { 432 ,  "sys_fsmount"                 },
  { 433 ,  "sys_fspick"                  },
  { 434 ,  "sys_pidfd_open"              },
  { 435 ,  "sys_clone3"                  },
  { 436 ,  "sys_close_range"             },
  { 437 ,  "sys_openat2"                 },
  { 438 ,  "sys_pidfd_getfd"             },
  { 439 ,  "sys_faccessat2"              },
  { 440 ,  "sys_process_madvise"         },
  { 441 ,  "sys_epoll_pwait2"            },
  { 442 ,  "sys_mount_setattr"           },
  { 443 ,  "sys_quotactl_fd"             },
  { 444 ,  "sys_landlock_create_ruleset" },
  { 445 ,  "sys_landlock_add_rule"       },
  { 446 ,  "sys_landlock_restrict_self"  },
  { 447 ,  "sys_memfd_secret"            },
  { 448 ,  "sys_process_mrelease"        },
};

int8_t 
get_sysmov_eax_value(const char* syscall)
{
  int hash_value = sys_has_key(syscall);
  return hash_value == -1 ? hash_value : syscall_value_table[hash_value].eax_value;
}
