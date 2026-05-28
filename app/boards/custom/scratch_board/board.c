#include <zephyr/init.h>
#include <zephyr/kernel.h>

static int board_scratch_board_init(void)
{
        printk("Board 'scratch_board' initialized!\n");

        return 0;
}
SYS_INIT(board_scratch_board_init, POST_KERNEL, 0);