/*
 **************************************************************************
 * Copyright (c) 2014, The Linux Foundation. All rights reserved.
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all copies.
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 **************************************************************************
 */

#include "nss_qdisc.h"

struct nss_fifo_sched_data {
	struct nss_qdisc nq;	/* Common base class for all nss qdiscs */
	u32 limit;			/* Queue length in packets */
					/* TODO: Support for queue length in bytes */
	u8 set_default;			/* Flag to set qdisc as default qdisc for enqueue */
};

static int nss_fifo_enqueue(struct sk_buff *skb, struct Qdisc *sch)
{
	return nss_qdisc_enqueue(skb, sch);
}

static struct sk_buff *nss_fifo_dequeue(struct Qdisc *sch)
{
	return nss_qdisc_dequeue(sch);
}

static unsigned int nss_fifo_drop(struct Qdisc *sch)
{
	nss_qdisc_info("nss_fifo dropping");
	return nss_qdisc_drop(sch);
}

static void nss_fifo_reset(struct Qdisc *sch)
{
	nss_qdisc_info("nss_fifo resetting!");
	nss_qdisc_reset(sch);
}

static void nss_fifo_destroy(struct Qdisc *sch)
{
	struct nss_qdisc *nq = (struct nss_qdisc *)qdisc_priv(sch);

	/*
	 * Stop the polling of basic stats
	 */
	nss_qdisc_stop_basic_stats_polling(nq);

	nss_qdisc_destroy(nq);
	nss_qdisc_info("nss_fifo destroyed");
}

static const struct nla_policy nss_fifo_policy[TCA_NSSFIFO_MAX + 1] = {
	[TCA_NSSFIFO_PARMS] = { .len = sizeof(struct tc_nssfifo_qopt) },
};

static int nss_fifo_change(struct Qdisc *sch, struct nlattr *opt)
{
	struct nss_fifo_sched_data *q;
	struct nlattr *na[TCA_NSSFIFO_MAX + 1];
	struct tc_nssfifo_qopt *qopt;
	int err;
	struct nss_if_msg nim;

	q = qdisc_priv(sch);

	if (opt == NULL) {
		return -EINVAL;
	}

	err = nla_parse_nested(na, TCA_NSSFIFO_MAX, opt, nss_fifo_policy);
	if (err < 0)
		return err;

	if (na[TCA_NSSFIFO_PARMS] == NULL)
		return -EINVAL;

	qopt = nla_data(na[TCA_NSSFIFO_PARMS]);

	if (!qopt->limit)
		qopt->limit = qdisc_dev(sch)->tx_queue_len ? : 1;

	q->limit = qopt->limit;

	/*
	 * Required for basic stats display
	 */
	sch->limit = qopt->limit;

	q->set_default = qopt->set_default;
	nss_qdisc_info("%s: limit:%u set_default:%u\n", __func__, qopt->limit, qopt->set_default);

	nim.msg.shaper_configure.config.msg.shaper_node_config.qos_tag = q->nq.qos_tag;
	nim.msg.shaper_configure.config.msg.shaper_node_config.snc.fifo_param.limit = q->limit;
	nim.msg.shaper_configure.config.msg.shaper_node_config.snc.fifo_param.drop_mode = NSS_SHAPER_FIFO_DROP_MODE_TAIL;
	if (nss_qdisc_configure(&q->nq, &nim, NSS_SHAPER_CONFIG_TYPE_FIFO_CHANGE_PARAM) < 0) {
		nss_qdisc_error("%s: nss_fifo %p configuration failed\n", __func__, sch);
		return -EINVAL;
	}

	/*
	 * There is nothing we need to do if the qdisc is not
	 * set as default qdisc.
	 */
	if (q->set_default == 0)
		return 0;

	/*
	 * Set this qdisc to be the default qdisc for enqueuing packets.
	 */
	if (nss_qdisc_set_default(&q->nq) < 0) {
		nss_qdisc_error("%s: nss_fifo %p set_default failed\n", __func__, sch);
		return -EINVAL;
	}

	nss_qdisc_info("%s: nss_fifo queue (qos_tag:%u) set as default\n", __func__, q->nq.qos_tag);
	return 0;
}

static int nss_fifo_init(struct Qdisc *sch, struct nlattr *opt)
{
	struct nss_qdisc *nq = qdisc_priv(sch);

	if (opt == NULL)
		return -EINVAL;

	nss_qdisc_info("Initializing Fifo - type %d\n", NSS_SHAPER_NODE_TYPE_FIFO);
	nss_fifo_reset(sch);

	if (nss_qdisc_init(sch, nq, NSS_SHAPER_NODE_TYPE_FIFO, 0) < 0)
		return -EINVAL;

	nss_qdisc_info("NSS fifo initialized - handle %x parent %x\n", sch->handle, sch->parent);
	if (nss_fifo_change(sch, opt) < 0) {
		nss_qdisc_destroy(nq);
		return -EINVAL;
	}

	/*
	 * Start the stats polling timer
	 */
	nss_qdisc_start_basic_stats_polling(nq);

	return 0;
}

static int nss_fifo_dump(struct Qdisc *sch, struct sk_buff *skb)
{
	struct nss_fifo_sched_data *q;
	struct nlattr *opts = NULL;
	struct tc_nssfifo_qopt opt;

	nss_qdisc_info("Nssfifo Dumping!");

	q = qdisc_priv(sch);
	if (q == NULL) {
		return -1;
	}

	opt.limit = q->limit;
	opt.set_default = q->set_default;

	opts = nla_nest_start(skb, TCA_OPTIONS);
	if (opts == NULL) {
		goto nla_put_failure;
	}
	if (nla_put(skb, TCA_NSSFIFO_PARMS, sizeof(opt), &opt))
		goto nla_put_failure;

	return nla_nest_end(skb, opts);

nla_put_failure:
	nla_nest_cancel(skb, opts);
	return -EMSGSIZE;
}

static struct sk_buff *nss_fifo_peek(struct Qdisc *sch)
{
	nss_qdisc_info("Nssfifo Peeking");
	return nss_qdisc_peek(sch);
}

struct Qdisc_ops nss_pfifo_qdisc_ops __read_mostly = {
	.id		=	"nsspfifo",
	.priv_size	=	sizeof(struct nss_fifo_sched_data),
	.enqueue	=	nss_fifo_enqueue,
	.dequeue	=	nss_fifo_dequeue,
	.peek		=	nss_fifo_peek,
	.drop		=	nss_fifo_drop,
	.init		=	nss_fifo_init,
	.reset		=	nss_fifo_reset,
	.destroy	=	nss_fifo_destroy,
	.change		=	nss_fifo_change,
	.dump		=	nss_fifo_dump,
	.owner		=	THIS_MODULE,
};

struct Qdisc_ops nss_bfifo_qdisc_ops __read_mostly = {
	.id		=	"nssbfifo",
	.priv_size	=	sizeof(struct nss_fifo_sched_data),
	.enqueue	=	nss_fifo_enqueue,
	.dequeue	=	nss_fifo_dequeue,
	.peek		=	nss_fifo_peek,
	.drop		=	nss_fifo_drop,
	.init		=	nss_fifo_init,
	.reset		=	nss_fifo_reset,
	.destroy	=	nss_fifo_destroy,
	.change		=	nss_fifo_change,
	.dump		=	nss_fifo_dump,
	.owner		=	THIS_MODULE,
};
