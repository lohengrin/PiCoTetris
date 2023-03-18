/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* This ifdef allows the header to be used from both C and C++. */
#ifdef __cplusplus
extern "C" {
#endif
/*
 * \brief Initialise BTstack example with cyw43
 *
 * \return 0 if ok
 */
int picow_bt_init(void);

/*
 * \brief Run the BTstack example
 *
 */
void picow_bt_main(void);

#ifdef __cplusplus
}
#endif
