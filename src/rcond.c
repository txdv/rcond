/*
 * Copyright (C) 2014 Sartura, Ltd.
 *
 * Author: Luka Perkov <luka.perkov@sartura.hr>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <uv.h>

#include "rcond.h"
#include "misc.h"

#include "tcp-client-tcp-client.h"
#include "tcp-server-tcp-server.h"

static void on_signal(uv_signal_t *handle, int signum)
{
	tcp_server_tcp_server_close();

	uv_stop(uv_default_loop());

	__debug("done");
}

int main(int argc, char *const argv[])
{
	int rc = 0;

	/* make sure we can allocate default loop */
	if (!uv_default_loop()) {
		__debug("failed to allocate default loop");
		return EXIT_FAILURE;
	}

	uv_signal_t sigterm;
	uv_signal_init(uv_default_loop(), &sigterm);
	uv_signal_start(&sigterm, on_signal, SIGTERM);
	uv_unref((uv_handle_t *) &sigterm);

	uv_signal_t sigint;
	uv_signal_init(uv_default_loop(), &sigint);
	uv_signal_start(&sigint, on_signal, SIGINT);
	uv_unref((uv_handle_t *) &sigint);

	rc = tcp_client_tcp_client_init("127.0.0.1", 10001, "127.0.0.1", 10002);
	if (rc)	goto exit;

	rc = tcp_server_tcp_server_init("127.0.0.1", 10003, "127.0.0.1", 10004);
	if (rc)	goto exit;

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	__debug("rcond has stopped");

	rc = EXIT_SUCCESS;
exit:
	uv_loop_close(uv_default_loop());

	return rc;
}
