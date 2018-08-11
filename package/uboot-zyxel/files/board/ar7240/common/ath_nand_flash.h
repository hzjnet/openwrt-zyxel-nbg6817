#ifndef _NAND_API_H
#define _NAND_API_H

struct nand_api {
    void (*_nand_init)(void);
    void (*_nand_read)(void);
};

void
nand_module_install(struct nand_api *api);

#define ATH_NAND_FLASH_BASE	0x1b000000u

#define ATH_NF_RST		(ATH_NAND_FLASH_BASE + 0x200u)
#define ATH_NF_CTRL		(ATH_NAND_FLASH_BASE + 0x204u)
#define ATH_NF_RST_REG		(ATH_NAND_FLASH_BASE + 0x208u)
#define ATH_NF_INT_MASK		(ATH_NAND_FLASH_BASE + 0x20cu)
#define ATH_NF_INT_STATUS	(ATH_NAND_FLASH_BASE + 0x210u)
#define ATH_NF_ECC_CTRL		(ATH_NAND_FLASH_BASE + 0x214u)
#define ATH_NF_ECC_OFFSET	(ATH_NAND_FLASH_BASE + 0x218u)
#define ATH_NF_ADDR0_0		(ATH_NAND_FLASH_BASE + 0x21cu)
#define ATH_NF_ADDR1_0		(ATH_NAND_FLASH_BASE + 0x220u)
#define ATH_NF_ADDR0_1		(ATH_NAND_FLASH_BASE + 0x224u)
#define ATH_NF_ADDR1_1		(ATH_NAND_FLASH_BASE + 0x228u)
#define ATH_NF_SPARE_SIZE	(ATH_NAND_FLASH_BASE + 0x230u)
#define ATH_NF_PROTECT		(ATH_NAND_FLASH_BASE + 0x238u)
#define ATH_NF_LOOKUP_EN	(ATH_NAND_FLASH_BASE + 0x240u)
#define ATH_NF_LOOKUP0		(ATH_NAND_FLASH_BASE + 0x244u)
#define ATH_NF_LOOKUP1		(ATH_NAND_FLASH_BASE + 0x248u)
#define ATH_NF_LOOKUP2		(ATH_NAND_FLASH_BASE + 0x24cu)
#define ATH_NF_LOOKUP3		(ATH_NAND_FLASH_BASE + 0x250u)
#define ATH_NF_LOOKUP4		(ATH_NAND_FLASH_BASE + 0x254u)
#define ATH_NF_LOOKUP5		(ATH_NAND_FLASH_BASE + 0x258u)
#define ATH_NF_LOOKUP6		(ATH_NAND_FLASH_BASE + 0x25cu)
#define ATH_NF_LOOKUP7		(ATH_NAND_FLASH_BASE + 0x260u)
#define ATH_NF_DMA_ADDR		(ATH_NAND_FLASH_BASE + 0x264u)
#define ATH_NF_DMA_COUNT	(ATH_NAND_FLASH_BASE + 0x268u)
#define ATH_NF_DMA_CTRL		(ATH_NAND_FLASH_BASE + 0x26cu)
#define ATH_NF_MEM_CTRL		(ATH_NAND_FLASH_BASE + 0x280u)
#define ATH_NF_PG_SIZE		(ATH_NAND_FLASH_BASE + 0x284u)
#define ATH_NF_RD_STATUS	(ATH_NAND_FLASH_BASE + 0x288u)
#define ATH_NF_TIME_SEQ		(ATH_NAND_FLASH_BASE + 0x28cu)
#define ATH_NF_TIMINGS_ASYN	(ATH_NAND_FLASH_BASE + 0x290u)
#define ATH_NF_TIMINGS_SYN	(ATH_NAND_FLASH_BASE + 0x294u)
#define ATH_NF_FIFO_DATA	(ATH_NAND_FLASH_BASE + 0x298u)
#define ATH_NF_TIME_MODE	(ATH_NAND_FLASH_BASE + 0x29cu)
#define ATH_NF_DMA_ADDR_OFFSET	(ATH_NAND_FLASH_BASE + 0x2a0u)
#define ATH_NF_FIFO_INIT	(ATH_NAND_FLASH_BASE + 0x2b0u)
#define ATH_NF_GENERIC_SEQ_CTRL	(ATH_NAND_FLASH_BASE + 0x2b4u)

#define ATH_NF_BLK_SIZE_S	0x11
#define ATH_NF_BLK_SIZE		(1 << ATH_NF_BLK_SIZE_S)
#define ATH_NF_BLK_SIZE_M	(ATH_NF_BLK_SIZE - 1)

#define ATH_NF_TIMING_ASYN	0x0
#define ATH_NF_STATUS_OK	0xc0
#define ATH_NF_RD_STATUS_MASK	0xc7

#define ATH_NF_CTRL_SMALL_BLOCK_EN	(1 << 21)

#define ATH_NF_CTRL_ADDR_CYCLE1_0	(0 << 18)
#define ATH_NF_CTRL_ADDR_CYCLE1_1	(1 << 18)
#define ATH_NF_CTRL_ADDR_CYCLE1_2	(2 << 18)
#define ATH_NF_CTRL_ADDR_CYCLE1_3	(3 << 18)
#define ATH_NF_CTRL_ADDR_CYCLE1_4	(4 << 18)
#define ATH_NF_CTRL_ADDR_CYCLE1_5	(5 << 18)

#define ATH_NF_CTRL_ADDR1_AUTO_INC_EN	(1 << 17)
#define ATH_NF_CTRL_ADDR0_AUTO_INC_EN	(1 << 16)
#define ATH_NF_CTRL_WORK_MODE_SYNC	(1 << 15)
#define ATH_NF_CTRL_PROT_EN		(1 << 14)
#define ATH_NF_CTRL_LOOKUP_EN		(1 << 13)
#define ATH_NF_CTRL_IO_WIDTH_16BIT	(1 << 12)
#define ATH_NF_CTRL_CUSTOM_SIZE_EN	(1 << 11)

#define ATH_NF_CTRL_PAGE_SIZE_256	(0 <<  8)	/* bytes */
#define ATH_NF_CTRL_PAGE_SIZE_512	(1 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_1024	(2 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_2048	(3 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_4096	(4 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_8192	(5 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_16384	(6 <<  8)
#define ATH_NF_CTRL_PAGE_SIZE_0		(7 <<  8)

#define ATH_NF_CTRL_BLOCK_SIZE_32	(0 <<  6)	/* pages */
#define ATH_NF_CTRL_BLOCK_SIZE_64	(1 <<  6)
#define ATH_NF_CTRL_BLOCK_SIZE_128	(2 <<  6)
#define ATH_NF_CTRL_BLOCK_SIZE_256	(3 <<  6)

#define ATH_NF_CTRL_ECC_EN		(1 <<  5)
#define ATH_NF_CTRL_INT_EN		(1 <<  4)
#define ATH_NF_CTRL_SPARE_EN		(1 <<  3)

#define ATH_NF_CTRL_ADDR_CYCLE0_0	(0 <<  0)
#define ATH_NF_CTRL_ADDR_CYCLE0_1	(1 <<  0)
#define ATH_NF_CTRL_ADDR_CYCLE0_2	(2 <<  0)
#define ATH_NF_CTRL_ADDR_CYCLE0_3	(3 <<  0)
#define ATH_NF_CTRL_ADDR_CYCLE0_4	(4 <<  0)
#define ATH_NF_CTRL_ADDR_CYCLE0_5	(5 <<  0)

#endif
