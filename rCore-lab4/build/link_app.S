.section .data
    .global _num_app
    .align 3
_num_app:
    .quad  5
    .quad app_0_start
    .quad app_1_start
    .quad app_2_start
    .quad app_3_start
    .quad app_4_start
    .quad app_4_end

    .global _app_names
_app_names:
    .string "task2"
    .string "user_shell"
    .string "initproc"
    .string "task0"
    .string "task1"

    .align 3
    .section .data
    .global app_0_start
    .global app_0_end
app_0_start:
    .incbin "build/task2"
app_0_end:

    .align 3
    .section .data
    .global app_1_start
    .global app_1_end
app_1_start:
    .incbin "build/user_shell"
app_1_end:

    .align 3
    .section .data
    .global app_2_start
    .global app_2_end
app_2_start:
    .incbin "build/initproc"
app_2_end:

    .align 3
    .section .data
    .global app_3_start
    .global app_3_end
app_3_start:
    .incbin "build/task0"
app_3_end:

    .align 3
    .section .data
    .global app_4_start
    .global app_4_end
app_4_start:
    .incbin "build/task1"
app_4_end:
