/*
 * RISC-V translation routines for the RVXI Base Integer Instruction Set.
 *
 * Copyright (c) 2020 Western Digital
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef CONFIG_USER_ONLY
#define do_hlv(ctx, a, func)  false
#define do_hsv(ctx, a, func)  false
#else
static void gen_helper_hyp_hlv_b(TCGv r, TCGv_env e, TCGv a)
{
    gen_helper_hyp_hlv_bu(r, e, a);
    tcg_gen_ext8s_tl(r, r);
}

static void gen_helper_hyp_hlv_h(TCGv r, TCGv_env e, TCGv a)
{
    gen_helper_hyp_hlv_hu(r, e, a);
    tcg_gen_ext16s_tl(r, r);
}

static void gen_helper_hyp_hlv_w(TCGv r, TCGv_env e, TCGv a)
{
    gen_helper_hyp_hlv_wu(r, e, a);
    tcg_gen_ext32s_tl(r, r);
}

static bool do_hlv(DisasContext *ctx, arg_r2 *a,
                   void (*func)(TCGv, TCGv_env, TCGv))
{
    TCGv dest = dest_gpr(ctx, a->rd);
    TCGv addr = get_gpr(ctx, a->rs1, EXT_NONE);

    decode_save_opc(ctx, 0);
    func(dest, tcg_env, addr);
    gen_set_gpr(ctx, a->rd, dest);
    return true;
}

static bool do_hsv(DisasContext *ctx, arg_r2_s *a,
                   void (*func)(TCGv_env, TCGv, TCGv))
{
    TCGv addr = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv data = get_gpr(ctx, a->rs2, EXT_NONE);

    decode_save_opc(ctx, 0);
    func(tcg_env, addr, data);
    return true;
}
#endif /* CONFIG_USER_ONLY */

static bool trans_hlv_b(DisasContext *ctx, arg_hlv_b *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_b);
}

static bool trans_hlv_h(DisasContext *ctx, arg_hlv_h *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_h);
}

static bool trans_hlv_w(DisasContext *ctx, arg_hlv_w *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_w);
}

static bool trans_hlv_bu(DisasContext *ctx, arg_hlv_bu *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_bu);
}

static bool trans_hlv_hu(DisasContext *ctx, arg_hlv_hu *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_hu);
}

static bool trans_hsv_b(DisasContext *ctx, arg_hsv_b *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hsv(ctx, a, gen_helper_hyp_hsv_b);
}

static bool trans_hsv_h(DisasContext *ctx, arg_hsv_h *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hsv(ctx, a, gen_helper_hyp_hsv_h);
}

static bool trans_hsv_w(DisasContext *ctx, arg_hsv_w *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hsv(ctx, a, gen_helper_hyp_hsv_w);
}

static bool trans_hlv_wu(DisasContext *ctx, arg_hlv_wu *a)
{
    REQUIRE_64BIT(ctx);
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_wu);
}

static bool trans_hlv_d(DisasContext *ctx, arg_hlv_d *a)
{
    REQUIRE_64BIT(ctx);
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlv_d);
}

static bool trans_hsv_d(DisasContext *ctx, arg_hsv_d *a)
{
    REQUIRE_64BIT(ctx);
    REQUIRE_EXT(ctx, RVH);
    return do_hsv(ctx, a, gen_helper_hyp_hsv_d);
}

static bool trans_hlvx_hu(DisasContext *ctx, arg_hlvx_hu *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlvx_hu);
}

static bool trans_hlvx_wu(DisasContext *ctx, arg_hlvx_wu *a)
{
    REQUIRE_EXT(ctx, RVH);
    return do_hlv(ctx, a, gen_helper_hyp_hlvx_wu);
}

static bool trans_hfence_gvma(DisasContext *ctx, arg_sfence_vma *a)
{
    REQUIRE_EXT(ctx, RVH);
#ifndef CONFIG_USER_ONLY
    decode_save_opc(ctx, 0);
    gen_helper_hyp_gvma_tlb_flush(tcg_env);
    return true;
#endif
    return false;
}

static bool trans_hfence_vvma(DisasContext *ctx, arg_sfence_vma *a)
{
    REQUIRE_EXT(ctx, RVH);
#ifndef CONFIG_USER_ONLY
    decode_save_opc(ctx, 0);
    gen_helper_hyp_tlb_flush(tcg_env);
    return true;
#endif
    return false;
}
