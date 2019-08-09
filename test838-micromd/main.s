.LCPI0_0:
.LCPI0_1:
.LCPI0_2:
.LCPI0_3:
.LCPI0_4:
.LCPI0_5:
.LCPI0_6:
.LCPI0_7:
.LCPI0_8:
.LCPI0_9:
.LCPI0_13:
.LCPI0_10:
.LCPI0_11:
.LCPI0_12:
main:                                   # @main
.Lfunc_begin0:
# %bb.0:
	push	rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 2680
	movabs	rax, 6364136223846793005
	mov	qword ptr [rsp + 176], 5489
	mov	ecx, 1
	mov	esi, 5489
	mov	edx, 2
	jmp	.LBB0_1
.LBB0_60:                               #   in Loop: Header=BB0_1 Depth=1
	lea	rsi, [rcx + 1]
	mov	rdi, rbp
	shr	rdi, 62
	xor	rdi, rbp
	imul	rdi, rax
	add	rsi, rdi
	add	rdi, rdx
	mov	qword ptr [rsp + 8*rdx + 176], rdi
	add	rcx, 2
	add	rdx, 2
.LBB0_1:                                # =>This Inner Loop Header: Depth=1
	mov	rdi, rsi
	shr	rdi, 62
	xor	rdi, rsi
	imul	rdi, rax
	lea	rbp, [rdx + rdi]
	add	rbp, -1
	mov	qword ptr [rsp + 8*rdx + 168], rbp
	cmp	rdx, 312
	jne	.LBB0_60
# %bb.2:
	mov	qword ptr [rsp + 2672], 0
	lea	rdi, [rsp + 16]
	call	md::system::system()
	mov	rbp, qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value]
	mov	rbx, qword ptr [rsp + 24]
	cmp	qword ptr [rbx + 8*rbp], 0
	jne	.LBB0_11
# %bb.3:
	mov	rax, qword ptr [rip + radius_attribute]
	mov	r14, qword ptr [rsp + 16]
	test	rax, rax
	je	.LBB0_4
# %bb.5:
.Ltmp0:
	xor	edi, edi
	call	rax
.Ltmp1:
	jmp	.LBB0_6
.LBB0_4:
	xorpd	xmm0, xmm0
.LBB0_6:
.Ltmp2:
	lea	rdi, [rsp + 80]
	mov	rsi, r14
	call	std::__1::unique_ptr<md::detail::array_erasure::instance<double>, std::__1::default_delete<md::detail::array_erasure::instance<double> > > md::detail::array_erasure::make<double>(unsigned long, double)
.Ltmp3:
# %bb.7:
	mov	rax, qword ptr [rsp + 80]
	mov	qword ptr [rsp + 80], 0
	mov	rdi, qword ptr [rbx + 8*rbp]
	mov	qword ptr [rbx + 8*rbp], rax
	test	rdi, rdi
	je	.LBB0_9
# %bb.8:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB0_9:
	mov	rdi, qword ptr [rsp + 80]
	mov	qword ptr [rsp + 80], 0
	test	rdi, rdi
	je	.LBB0_11
# %bb.10:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB0_11:
	movabs	r9, -5403634167711393303
	xor	ecx, ecx
	movabs	r14, 945986875574848801
	mov	r12, -2147483648
	xor	esi, esi
.LBB0_12:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_14 Depth 2
	mov	dword ptr [rsp + 12], esi # 4-byte Spill
	lea	rsi, [rcx + 1]
	mov	rax, rsi
	shr	rax, 3
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	sub	rsi, rax
	mov	rax, qword ptr [rsp + 8*rcx + 176]
	and	rax, r12
	mov	rdi, qword ptr [rsp + 8*rsi + 176]
	mov	ebp, edi
	and	ebp, 2147483646
	or	rbp, rax
	lea	rax, [rcx + 156]
	shr	rax, 3
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	neg	rax
	add	rax, rcx
	add	rax, 156
	shr	rbp
	and	edi, 1
	imul	rdi, r9
	xor	rdi, qword ptr [rsp + 8*rax + 176]
	xor	rdi, rbp
	mov	qword ptr [rsp + 8*rcx + 176], rdi
	mov	rax, qword ptr [rsp + 2672]
	mov	r15, qword ptr [rsp + 8*rax + 176]
	lea	rcx, [rsi + 1]
	mov	rax, rcx
	shr	rax, 3
	mul	r14
	mov	qword ptr [rsp + 2672], rsi
	shr	rdx
	imul	rax, rdx, 312
	sub	rcx, rax
	mov	rdi, qword ptr [rsp + 8*rsi + 176]
	and	rdi, r12
	mov	rbp, qword ptr [rsp + 8*rcx + 176]
	mov	ebx, ebp
	and	ebx, 2147483646
	lea	rax, [rsi + 156]
	shr	rax, 3
	mul	r14
	or	rbx, rdi
	shr	rdx
	imul	rax, rdx, 312
	neg	rax
	add	rax, rsi
	add	rax, 156
	shr	rbx
	and	ebp, 1
	imul	rbp, r9
	xor	rbp, qword ptr [rsp + 8*rax + 176]
	xor	rbp, rbx
	mov	qword ptr [rsp + 8*rsi + 176], rbp
	mov	rax, qword ptr [rsp + 2672]
	mov	r13, qword ptr [rsp + 8*rax + 176]
	mov	qword ptr [rsp + 2672], rcx
	lea	rax, [rcx + 1]
	shr	rax, 3
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	neg	rax
	lea	rbp, [rcx + rax]
	add	rbp, 1
	mov	rax, qword ptr [rsp + 8*rcx + 176]
	and	rax, r12
	mov	rsi, qword ptr [rsp + 8*rbp + 176]
	mov	edi, esi
	and	edi, 2147483646
	or	rdi, rax
	lea	rbx, [rcx + 156]
	mov	rax, rbx
	shr	rax, 3
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	sub	rbx, rax
	shr	rdi
	and	esi, 1
	imul	rsi, r9
	xor	rsi, qword ptr [rsp + 8*rbx + 176]
	xor	rsi, rdi
	mov	qword ptr [rsp + 8*rcx + 176], rsi
	mov	rax, qword ptr [rsp + 2672]
	mov	rcx, qword ptr [rsp + 8*rax + 176]
	mov	qword ptr [rsp + 2672], rbp
	xorpd	xmm0, xmm0
	movapd	xmmword ptr [rsp + 80], xmm0
	mov	qword ptr [rsp + 96], 0
	mov	rax, qword ptr [rsp + 16]
	mov	r14, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 152], rax # 8-byte Spill
	lea	rbx, [rax + 1]
	mov	r12, qword ptr [rsp + 32]
	cmp	r14, r12
	je	.LBB0_18
# %bb.13:                               #   in Loop: Header=BB0_12 Depth=1
	mov	qword ptr [rsp + 144], rcx # 8-byte Spill
.LBB0_14:                               #   Parent Loop BB0_12 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	mov	rdi, qword ptr [r14]
	test	rdi, rdi
	je	.LBB0_16
# %bb.15:                               #   in Loop: Header=BB0_14 Depth=2
	mov	rax, qword ptr [rdi]
.Ltmp5:
	mov	rsi, rbx
	call	qword ptr [rax + 24]
.Ltmp6:
.LBB0_16:                               #   in Loop: Header=BB0_14 Depth=2
	add	r14, 8
	cmp	r12, r14
	jne	.LBB0_14
# %bb.17:                               #   in Loop: Header=BB0_12 Depth=1
	mov	r14, qword ptr [rsp + 24]
	mov	rcx, qword ptr [rsp + 144] # 8-byte Reload
.LBB0_18:                               #   in Loop: Header=BB0_12 Depth=1
	movq	xmm0, r15
	shr	r15, 29
	movabs	r11, 22906492245
	and	r15, r11
	movq	xmm1, r13
	shr	r13, 29
	and	r13, r11
	movq	xmm2, r15
	movq	xmm3, r13
	punpcklqdq	xmm2, xmm3      # xmm2 = xmm2[0],xmm3[0]
	punpcklqdq	xmm0, xmm1      # xmm0 = xmm0[0],xmm1[0]
	pxor	xmm0, xmm2
	movdqa	xmm1, xmm0
	psllq	xmm1, 17
	pand	xmm1, xmmword ptr [rip + .LCPI0_0]
	pxor	xmm1, xmm0
	movdqa	xmm2, xmm1
	psllq	xmm2, 37
	pand	xmm2, xmmword ptr [rip + .LCPI0_1]
	pxor	xmm2, xmm1
	movdqa	xmm0, xmm2
	psrlq	xmm0, 43
	pxor	xmm0, xmm2
	movdqa	xmm1, xmm0
	pand	xmm1, xmmword ptr [rip + .LCPI0_2]
	por	xmm1, xmmword ptr [rip + .LCPI0_3]
	psrlq	xmm0, 32
	por	xmm0, xmmword ptr [rip + .LCPI0_4]
	subpd	xmm0, xmmword ptr [rip + .LCPI0_5]
	addpd	xmm0, xmm1
	mov	rax, rcx
	shr	rax, 29
	and	rax, r11
	xor	rax, rcx
	mov	rcx, rax
	shl	rcx, 17
	movabs	r10, 8202884508482404352
	and	rcx, r10
	xor	rcx, rax
	mov	rax, rcx
	shl	rax, 37
	movabs	r8, -2270628950310912
	and	rax, r8
	xor	rax, rcx
	mov	rcx, rax
	shr	rcx, 43
	xor	rcx, rax
	movq	xmm1, rcx
	movdqa	xmm3, xmmword ptr [rip + .LCPI0_8] # xmm3 = [1127219200,1160773632,0,0]
	punpckldq	xmm1, xmm3      # xmm1 = xmm1[0],xmm3[0],xmm1[1],xmm3[1]
	movapd	xmm4, xmmword ptr [rip + .LCPI0_9] # xmm4 = [4.503599627370496E+15,1.9342813113834067E+25]
	subpd	xmm1, xmm4
	movapd	xmm2, xmm1
	unpckhpd	xmm2, xmm1      # xmm2 = xmm2[1],xmm1[1]
	addpd	xmm2, xmm1
	mulpd	xmm0, xmmword ptr [rip + .LCPI0_6]
	addpd	xmm0, xmm0
	addpd	xmm0, xmmword ptr [rip + .LCPI0_7]
	movsd	xmm5, qword ptr [rip + .LCPI0_10] # xmm5 = mem[0],zero
	mulsd	xmm2, xmm5
	addsd	xmm2, xmm2
	addsd	xmm2, qword ptr [rip + .LCPI0_11]
	mov	qword ptr [rsp + 16], rbx
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_mass_attribute>::value]
	mov	rax, qword ptr [r14 + 8*rax]
	mov	rax, qword ptr [rax + 8]
	movabs	rcx, 4607182418800017408
	mov	rbx, qword ptr [rsp + 152] # 8-byte Reload
	mov	qword ptr [rax + 8*rbx], rcx
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_mobility_attribute>::value]
	mov	rax, qword ptr [r14 + 8*rax]
	mov	rax, qword ptr [rax + 8]
	mov	qword ptr [rax + 8*rbx], rcx
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	rax, qword ptr [r14 + 8*rax]
	mov	rax, qword ptr [rax + 8]
	lea	rcx, [8*rbx]
	lea	rcx, [rcx + 2*rcx]
	movupd	xmmword ptr [rax + rcx], xmm0
	movsd	qword ptr [rax + rcx + 16], xmm2
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_velocity_attribute>::value]
	mov	rdx, qword ptr [rsp + 24]
	mov	rax, qword ptr [rdx + 8*rax]
	mov	rax, qword ptr [rax + 8]
	mov	rdx, qword ptr [rsp + 96]
	mov	qword ptr [rax + rcx + 16], rdx
	movaps	xmm0, xmmword ptr [rsp + 80]
	movups	xmmword ptr [rax + rcx], xmm0
	lea	rax, [rbp + 1]
	shr	rax, 3
	movabs	r14, 945986875574848801
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	neg	rax
	lea	rcx, [rax + rbp]
	add	rcx, 1
	mov	rsi, qword ptr [rsp + 8*rcx + 176]
	mov	edi, esi
	lea	rax, [rbp + 156]
	shr	rax, 3
	mul	r14
	shr	rdx
	imul	rax, rdx, 312
	neg	rax
	add	rax, rbp
	add	rax, 156
	and	esi, 1
	movabs	r9, -5403634167711393303
	imul	rsi, r9
	xor	rsi, qword ptr [rsp + 8*rax + 176]
	mov	rax, qword ptr [rsp + 8*rbp + 176]
	mov	r12, -2147483648
	and	rax, r12
	and	edi, 2147483646
	or	rdi, rax
	shr	rdi
	xor	rsi, rdi
	mov	qword ptr [rsp + 8*rbp + 176], rsi
	mov	rax, qword ptr [rsp + 2672]
	mov	rax, qword ptr [rsp + 8*rax + 176]
	mov	rdx, rax
	shr	rdx, 29
	and	rdx, r11
	xor	rdx, rax
	mov	rax, rdx
	shl	rax, 17
	and	rax, r10
	xor	rax, rdx
	mov	rdx, rax
	shl	rdx, 37
	and	rdx, r8
	xor	rdx, rax
	mov	rax, rdx
	shr	rax, 43
	xor	rax, rdx
	movq	xmm0, rax
	punpckldq	xmm0, xmm3      # xmm0 = xmm0[0],xmm3[0],xmm0[1],xmm3[1]
	subpd	xmm0, xmm4
	movapd	xmm1, xmm0
	unpckhpd	xmm1, xmm0      # xmm1 = xmm1[1],xmm0[1]
	addpd	xmm1, xmm0
	mov	rax, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 2672], rcx
	mov	rdx, qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value]
	mov	rax, qword ptr [rax + 8*rdx]
	mulsd	xmm1, xmm5
	addsd	xmm1, qword ptr [rip + .LCPI0_12]
	mov	rax, qword ptr [rax + 8]
	movsd	qword ptr [rax + 8*rbx], xmm1
	mov	esi, dword ptr [rsp + 12] # 4-byte Reload
	add	esi, 1
	cmp	esi, 100
	jb	.LBB0_12
# %bb.19:
	mov	qword ptr [rsp + 96], 0
	mov	eax, offset vtable for my_forcefield+128
	movq	xmm0, rax
	mov	eax, offset vtable for my_forcefield+48
	movq	xmm1, rax
	punpcklqdq	xmm1, xmm0      # xmm1 = xmm1[0],xmm0[0]
	movdqa	xmmword ptr [rsp + 80], xmm1
	pxor	xmm0, xmm0
	movdqu	xmmword ptr [rsp + 104], xmm0
	mov	qword ptr [rsp + 120], 0
	movabs	rax, 4607182418800017408
	mov	qword ptr [rsp + 128], rax
.Ltmp8:
	lea	rdi, [rsp + 160]
	lea	rsi, [rsp + 16]
	lea	rdx, [rsp + 80]
	call	std::__1::shared_ptr<my_forcefield> md::system::add_forcefield<my_forcefield, void>(my_forcefield const&)
.Ltmp9:
# %bb.20:
	mov	rbx, qword ptr [rsp + 168]
	test	rbx, rbx
	je	.LBB0_23
# %bb.21:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB0_23
# %bb.22:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB0_23:
	mov	qword ptr [rsp + 48], offset vtable for md::detail::sum_forcefield+16
	mov	r14, qword ptr [rsp + 56]
	test	r14, r14
	je	.LBB0_32
# %bb.24:
	mov	rbp, qword ptr [rsp + 64]
	cmp	rbp, r14
	je	.LBB0_25
.LBB0_26:                               # =>This Inner Loop Header: Depth=1
	mov	rbx, qword ptr [rbp - 8]
	add	rbp, -16
	test	rbx, rbx
	je	.LBB0_29
# %bb.27:                               #   in Loop: Header=BB0_26 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	je	.LBB0_28
.LBB0_29:                               #   in Loop: Header=BB0_26 Depth=1
	cmp	r14, rbp
	jne	.LBB0_26
	jmp	.LBB0_30
.LBB0_28:                               #   in Loop: Header=BB0_26 Depth=1
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
	cmp	r14, rbp
	jne	.LBB0_26
.LBB0_30:
	mov	rdi, qword ptr [rsp + 56]
	jmp	.LBB0_31
.LBB0_25:
	mov	rdi, r14
.LBB0_31:
	mov	qword ptr [rsp + 64], r14
	call	operator delete(void*)
.LBB0_32:
	mov	rbx, qword ptr [rsp + 24]
	test	rbx, rbx
	je	.LBB0_40
# %bb.33:
	mov	rbp, qword ptr [rsp + 32]
	cmp	rbp, rbx
	je	.LBB0_34
.LBB0_35:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbp - 8]
	mov	qword ptr [rbp - 8], 0
	lea	rbp, [rbp - 8]
	test	rdi, rdi
	je	.LBB0_37
# %bb.36:                               #   in Loop: Header=BB0_35 Depth=1
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB0_37:                               #   in Loop: Header=BB0_35 Depth=1
	cmp	rbx, rbp
	jne	.LBB0_35
# %bb.38:
	mov	rdi, qword ptr [rsp + 24]
	jmp	.LBB0_39
.LBB0_34:
	mov	rdi, rbx
.LBB0_39:
	mov	qword ptr [rsp + 32], rbx
	call	operator delete(void*)
.LBB0_40:
	xor	eax, eax
	add	rsp, 2680
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
.LBB0_61:
.Ltmp4:
	jmp	.LBB0_42
.LBB0_41:
.Ltmp10:
	jmp	.LBB0_42
.LBB0_62:
.Ltmp7:
.LBB0_42:
	mov	r14, rax
	mov	qword ptr [rsp + 48], offset vtable for md::detail::sum_forcefield+16
	mov	r15, qword ptr [rsp + 56]
	test	r15, r15
	je	.LBB0_51
# %bb.43:
	mov	rbp, qword ptr [rsp + 64]
	cmp	rbp, r15
	je	.LBB0_44
.LBB0_45:                               # =>This Inner Loop Header: Depth=1
	mov	rbx, qword ptr [rbp - 8]
	add	rbp, -16
	test	rbx, rbx
	je	.LBB0_48
# %bb.46:                               #   in Loop: Header=BB0_45 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB0_48
# %bb.47:                               #   in Loop: Header=BB0_45 Depth=1
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB0_48:                               #   in Loop: Header=BB0_45 Depth=1
	cmp	r15, rbp
	jne	.LBB0_45
# %bb.49:
	mov	rdi, qword ptr [rsp + 56]
	jmp	.LBB0_50
.LBB0_44:
	mov	rdi, r15
.LBB0_50:
	mov	qword ptr [rsp + 64], r15
	call	operator delete(void*)
.LBB0_51:
	mov	rbx, qword ptr [rsp + 24]
	test	rbx, rbx
	je	.LBB0_59
# %bb.52:
	mov	rbp, qword ptr [rsp + 32]
	cmp	rbp, rbx
	je	.LBB0_53
.LBB0_54:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbp - 8]
	mov	qword ptr [rbp - 8], 0
	lea	rbp, [rbp - 8]
	test	rdi, rdi
	je	.LBB0_56
# %bb.55:                               #   in Loop: Header=BB0_54 Depth=1
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB0_56:                               #   in Loop: Header=BB0_54 Depth=1
	cmp	rbx, rbp
	jne	.LBB0_54
# %bb.57:
	mov	rdi, qword ptr [rsp + 24]
.LBB0_58:
	mov	qword ptr [rsp + 32], rbx
	call	operator delete(void*)
.LBB0_59:
	mov	rdi, r14
	call	_Unwind_Resume
.LBB0_53:
	mov	rdi, rbx
	jmp	.LBB0_58
.Lfunc_end0:
GCC_except_table0:
.Lexception0:
.Lcst_begin0:
.Lcst_end0:
                                        # -- End function
.LCPI1_0:
md::system::system():                      # @md::system::system()
.Lfunc_begin1:
# %bb.0:
	push	rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 56
	mov	r13, rdi
	call	md::detail::attribute_table::attribute_table()
	mov	qword ptr [r13 + 32], offset vtable for md::detail::sum_forcefield+16
	lea	r12, [r13 + 40]
	xorps	xmm0, xmm0
	movups	xmmword ptr [r13 + 40], xmm0
	mov	qword ptr [r13 + 56], 0
	mov	rbp, qword ptr [rip + md::detail::type_hash::hash<md::tag_mass_attribute>::value]
	mov	rbx, qword ptr [r13 + 8]
	cmp	qword ptr [rbx + 8*rbp], 0
	jne	.LBB1_7
# %bb.1:
	mov	rsi, qword ptr [r13]
.Ltmp11:
	lea	rdi, [rsp + 32]
	movsd	xmm0, qword ptr [rip + .LCPI1_0] # xmm0 = mem[0],zero
	call	std::__1::unique_ptr<md::detail::array_erasure::instance<double>, std::__1::default_delete<md::detail::array_erasure::instance<double> > > md::detail::array_erasure::make<double>(unsigned long, double)
.Ltmp12:
# %bb.2:
	mov	rax, qword ptr [rsp + 32]
	mov	qword ptr [rsp + 32], 0
	mov	rdi, qword ptr [rbx + 8*rbp]
	mov	qword ptr [rbx + 8*rbp], rax
	test	rdi, rdi
	je	.LBB1_4
# %bb.3:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_4:
	mov	rdi, qword ptr [rsp + 32]
	mov	qword ptr [rsp + 32], 0
	test	rdi, rdi
	je	.LBB1_6
# %bb.5:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_6:
	mov	rbx, qword ptr [r13 + 8]
.LBB1_7:
	mov	rbp, qword ptr [rip + md::detail::type_hash::hash<md::tag_mobility_attribute>::value]
	cmp	qword ptr [rbx + 8*rbp], 0
	jne	.LBB1_14
# %bb.8:
	mov	rsi, qword ptr [r13]
.Ltmp13:
	lea	rdi, [rsp + 32]
	movsd	xmm0, qword ptr [rip + .LCPI1_0] # xmm0 = mem[0],zero
	call	std::__1::unique_ptr<md::detail::array_erasure::instance<double>, std::__1::default_delete<md::detail::array_erasure::instance<double> > > md::detail::array_erasure::make<double>(unsigned long, double)
.Ltmp14:
# %bb.9:
	mov	rax, qword ptr [rsp + 32]
	mov	qword ptr [rsp + 32], 0
	mov	rdi, qword ptr [rbx + 8*rbp]
	mov	qword ptr [rbx + 8*rbp], rax
	test	rdi, rdi
	je	.LBB1_11
# %bb.10:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_11:
	mov	rdi, qword ptr [rsp + 32]
	mov	qword ptr [rsp + 32], 0
	test	rdi, rdi
	je	.LBB1_13
# %bb.12:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_13:
	mov	rbx, qword ptr [r13 + 8]
.LBB1_14:
	mov	rbp, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	cmp	qword ptr [rbx + 8*rbp], 0
	jne	.LBB1_21
# %bb.15:
	mov	rsi, qword ptr [r13]
	xorps	xmm0, xmm0
	movaps	xmmword ptr [rsp + 32], xmm0
	mov	qword ptr [rsp + 48], 0
.Ltmp15:
	mov	rax, qword ptr [rsp + 48]
	mov	qword ptr [rsp + 16], rax
	movaps	xmm0, xmmword ptr [rsp + 32]
	movups	xmmword ptr [rsp], xmm0
	lea	rdi, [rsp + 24]
	call	std::__1::unique_ptr<md::detail::array_erasure::instance<md::point>, std::__1::default_delete<md::detail::array_erasure::instance<md::point> > > md::detail::array_erasure::make<md::point>(unsigned long, md::point)
.Ltmp16:
# %bb.16:
	mov	rax, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 24], 0
	mov	rdi, qword ptr [rbx + 8*rbp]
	mov	qword ptr [rbx + 8*rbp], rax
	test	rdi, rdi
	je	.LBB1_18
# %bb.17:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_18:
	mov	rdi, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 24], 0
	test	rdi, rdi
	je	.LBB1_20
# %bb.19:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_20:
	mov	rbx, qword ptr [r13 + 8]
.LBB1_21:
	mov	rbp, qword ptr [rip + md::detail::type_hash::hash<md::tag_velocity_attribute>::value]
	cmp	qword ptr [rbx + 8*rbp], 0
	jne	.LBB1_27
# %bb.22:
	mov	rsi, qword ptr [r13]
	xorps	xmm0, xmm0
	movaps	xmmword ptr [rsp + 32], xmm0
	mov	qword ptr [rsp + 48], 0
.Ltmp17:
	mov	rax, qword ptr [rsp + 48]
	mov	qword ptr [rsp + 16], rax
	movaps	xmm0, xmmword ptr [rsp + 32]
	movups	xmmword ptr [rsp], xmm0
	lea	rdi, [rsp + 24]
	call	std::__1::unique_ptr<md::detail::array_erasure::instance<md::vector>, std::__1::default_delete<md::detail::array_erasure::instance<md::vector> > > md::detail::array_erasure::make<md::vector>(unsigned long, md::vector)
.Ltmp18:
# %bb.23:
	mov	rax, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 24], 0
	mov	rdi, qword ptr [rbx + 8*rbp]
	mov	qword ptr [rbx + 8*rbp], rax
	test	rdi, rdi
	je	.LBB1_25
# %bb.24:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_25:
	mov	rdi, qword ptr [rsp + 24]
	mov	qword ptr [rsp + 24], 0
	test	rdi, rdi
	je	.LBB1_27
# %bb.26:
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_27:
	add	rsp, 56
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
.LBB1_28:
.Ltmp19:
	mov	r14, rax
	mov	qword ptr [r13 + 32], offset vtable for md::detail::sum_forcefield+16
	mov	rbx, qword ptr [r13 + 40]
	test	rbx, rbx
	je	.LBB1_37
# %bb.29:
	mov	rbp, qword ptr [r13 + 48]
	cmp	rbp, rbx
	je	.LBB1_30
.LBB1_31:                               # =>This Inner Loop Header: Depth=1
	mov	r15, qword ptr [rbp - 8]
	add	rbp, -16
	test	r15, r15
	je	.LBB1_34
# %bb.32:                               #   in Loop: Header=BB1_31 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [r15 + 8], rax
	test	rax, rax
	jne	.LBB1_34
# %bb.33:                               #   in Loop: Header=BB1_31 Depth=1
	mov	rax, qword ptr [r15]
	mov	rdi, r15
	call	qword ptr [rax + 16]
	mov	rdi, r15
	call	std::__1::__shared_weak_count::__release_weak()
.LBB1_34:                               #   in Loop: Header=BB1_31 Depth=1
	cmp	rbx, rbp
	jne	.LBB1_31
# %bb.35:
	mov	rdi, qword ptr [r12]
	jmp	.LBB1_36
.LBB1_30:
	mov	rdi, rbx
.LBB1_36:
	mov	qword ptr [r13 + 48], rbx
	call	operator delete(void*)
.LBB1_37:
	mov	rbx, qword ptr [r13 + 8]
	test	rbx, rbx
	je	.LBB1_45
# %bb.38:
	mov	rbp, qword ptr [r13 + 16]
	cmp	rbp, rbx
	je	.LBB1_39
.LBB1_40:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbp - 8]
	mov	qword ptr [rbp - 8], 0
	lea	rbp, [rbp - 8]
	test	rdi, rdi
	je	.LBB1_42
# %bb.41:                               #   in Loop: Header=BB1_40 Depth=1
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB1_42:                               #   in Loop: Header=BB1_40 Depth=1
	cmp	rbx, rbp
	jne	.LBB1_40
# %bb.43:
	mov	rdi, qword ptr [r13 + 8]
.LBB1_44:
	mov	qword ptr [r13 + 16], rbx
	call	operator delete(void*)
.LBB1_45:
	mov	rdi, r14
	call	_Unwind_Resume
.LBB1_39:
	mov	rdi, rbx
	jmp	.LBB1_44
.Lfunc_end1:
GCC_except_table1:
.Lexception1:
.Lcst_begin1:
.Lcst_end1:
                                        # -- End function
std::__1::shared_ptr<my_forcefield> md::system::add_forcefield<my_forcefield, void>(my_forcefield const&): # @std::__1::shared_ptr<my_forcefield> md::system::add_forcefield<my_forcefield, void>(my_forcefield const&)
.Lfunc_begin2:
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 16
	mov	rbx, rdx
	mov	r14, rsi
	mov	r15, rdi
	mov	edi, 80
	call	operator new(unsigned long)
	xorps	xmm0, xmm0
	movups	xmmword ptr [rax + 8], xmm0
	mov	qword ptr [rax], offset vtable for std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >+16
	lea	rcx, [rax + 24]
	mov	rdx, qword ptr [rbx + 48]
	mov	qword ptr [rax + 72], rdx
	movups	xmm0, xmmword ptr [rbx + 16]
	movups	xmm1, xmmword ptr [rbx + 32]
	movups	xmmword ptr [rax + 56], xmm1
	movups	xmmword ptr [rax + 40], xmm0
	mov	edx, offset vtable for my_forcefield+128
	movq	xmm0, rdx
	mov	edx, offset vtable for my_forcefield+48
	movq	xmm1, rdx
	punpcklqdq	xmm1, xmm0      # xmm1 = xmm1[0],xmm0[0]
	movdqu	xmmword ptr [rax + 24], xmm1
	mov	qword ptr [r15], rcx
	mov	qword ptr [r15 + 8], rax
	mov	qword ptr [rsp], rcx
	mov	qword ptr [rsp + 8], rax
	lock		add	qword ptr [rax + 8], 1
.Ltmp20:
	mov	rsi, rsp
	mov	rdi, r14
	call	md::system::add_forcefield(std::__1::shared_ptr<md::forcefield>)
.Ltmp21:
# %bb.1:
	mov	rbx, qword ptr [rsp + 8]
	test	rbx, rbx
	je	.LBB2_4
# %bb.2:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB2_4
# %bb.3:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB2_4:
	mov	rax, r15
	add	rsp, 16
	pop	rbx
	pop	r14
	pop	r15
	ret
.LBB2_5:
.Ltmp22:
	mov	r14, rax
	mov	rbx, qword ptr [rsp + 8]
	test	rbx, rbx
	je	.LBB2_8
# %bb.6:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB2_8
# %bb.7:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB2_8:
	mov	rbx, qword ptr [r15 + 8]
	test	rbx, rbx
	je	.LBB2_11
# %bb.9:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB2_11
# %bb.10:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB2_11:
	mov	rdi, r14
	call	_Unwind_Resume
.Lfunc_end2:
GCC_except_table2:
.Lexception2:
.Lcst_begin2:
.Lcst_end2:
                                        # -- End function
my_forcefield::~my_forcefield():                 # @my_forcefield::~my_forcefield()
# %bb.0:
	ret
.Lfunc_end3:
                                        # -- End function
__cxx_global_var_init:                  # @__cxx_global_var_init
# %bb.0:
	cmp	byte ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_mass_attribute>::value], 0
	je	.LBB4_1
# %bb.2:
	ret
.LBB4_1:
	mov	rax, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	lea	rcx, [rax + 1]
	mov	qword ptr [rip + md::detail::type_hash::get_counter()::counter], rcx
	mov	qword ptr [rip + md::detail::type_hash::hash<md::tag_mass_attribute>::value], rax
	mov	qword ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_mass_attribute>::value], 1
	ret
.Lfunc_end4:
                                        # -- End function
__cxx_global_var_init.1:                # @__cxx_global_var_init.1
# %bb.0:
	cmp	byte ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_mobility_attribute>::value], 0
	je	.LBB5_1
# %bb.2:
	ret
.LBB5_1:
	mov	rax, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	lea	rcx, [rax + 1]
	mov	qword ptr [rip + md::detail::type_hash::get_counter()::counter], rcx
	mov	qword ptr [rip + md::detail::type_hash::hash<md::tag_mobility_attribute>::value], rax
	mov	qword ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_mobility_attribute>::value], 1
	ret
.Lfunc_end5:
                                        # -- End function
__cxx_global_var_init.2:                # @__cxx_global_var_init.2
# %bb.0:
	cmp	byte ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_position_attribute>::value], 0
	je	.LBB6_1
# %bb.2:
	ret
.LBB6_1:
	mov	rax, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	lea	rcx, [rax + 1]
	mov	qword ptr [rip + md::detail::type_hash::get_counter()::counter], rcx
	mov	qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value], rax
	mov	qword ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_position_attribute>::value], 1
	ret
.Lfunc_end6:
                                        # -- End function
__cxx_global_var_init.3:                # @__cxx_global_var_init.3
# %bb.0:
	cmp	byte ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_velocity_attribute>::value], 0
	je	.LBB7_1
# %bb.2:
	ret
.LBB7_1:
	mov	rax, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	lea	rcx, [rax + 1]
	mov	qword ptr [rip + md::detail::type_hash::get_counter()::counter], rcx
	mov	qword ptr [rip + md::detail::type_hash::hash<md::tag_velocity_attribute>::value], rax
	mov	qword ptr [rip + guard variable for md::detail::type_hash::hash<md::tag_velocity_attribute>::value], 1
	ret
.Lfunc_end7:
                                        # -- End function
__cxx_global_var_init.4:                # @__cxx_global_var_init.4
# %bb.0:
	cmp	byte ptr [rip + guard variable for md::detail::type_hash::hash<radius_attribute_key>::value], 0
	je	.LBB8_1
# %bb.2:
	ret
.LBB8_1:
	mov	rax, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	lea	rcx, [rax + 1]
	mov	qword ptr [rip + md::detail::type_hash::get_counter()::counter], rcx
	mov	qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value], rax
	mov	qword ptr [rip + guard variable for md::detail::type_hash::hash<radius_attribute_key>::value], 1
	ret
.Lfunc_end8:
                                        # -- End function
md::detail::attribute_table::attribute_table():     # @md::detail::attribute_table::attribute_table()
.Lfunc_begin3:
# %bb.0:
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	mov	qword ptr [rdi], 0
	mov	r13, qword ptr [rip + md::detail::type_hash::get_counter()::counter]
	xorps	xmm0, xmm0
	movups	xmmword ptr [rdi + 8], xmm0
	mov	qword ptr [rdi + 24], 0
	test	r13, r13
	je	.LBB9_6
# %bb.1:
	mov	r12, rdi
	lea	r14, [rdi + 8]
	mov	rax, r13
	shr	rax, 61
	jne	.LBB9_2
# %bb.4:
	lea	r15, [8*r13]
.Ltmp23:
	mov	rdi, r15
	call	operator new(unsigned long)
.Ltmp24:
# %bb.5:
	mov	rbx, rax
	mov	qword ptr [r12 + 8], rax
	lea	rax, [rax + 8*r13]
	mov	qword ptr [r12 + 24], rax
	mov	rdi, rbx
	xor	esi, esi
	mov	rdx, r15
	call	memset
	add	rbx, r15
	mov	qword ptr [r12 + 16], rbx
.LBB9_6:
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
.LBB9_2:
.Ltmp25:
	mov	rdi, r14
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Ltmp26:
# %bb.3:
.LBB9_7:
.Ltmp27:
	mov	r15, rax
	mov	r13, qword ptr [r14]
	test	r13, r13
	je	.LBB9_15
# %bb.8:
	mov	rbx, qword ptr [r12 + 16]
	cmp	rbx, r13
	je	.LBB9_9
.LBB9_10:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbx - 8]
	mov	qword ptr [rbx - 8], 0
	lea	rbx, [rbx - 8]
	test	rdi, rdi
	je	.LBB9_12
# %bb.11:                               #   in Loop: Header=BB9_10 Depth=1
	mov	rax, qword ptr [rdi]
	call	qword ptr [rax + 8]
.LBB9_12:                               #   in Loop: Header=BB9_10 Depth=1
	cmp	r13, rbx
	jne	.LBB9_10
# %bb.13:
	mov	rdi, qword ptr [r14]
.LBB9_14:
	mov	qword ptr [r12 + 16], r13
	call	operator delete(void*)
.LBB9_15:
	mov	rdi, r15
	call	_Unwind_Resume
.LBB9_9:
	mov	rdi, r13
	jmp	.LBB9_14
.Lfunc_end9:
GCC_except_table9:
.Lexception3:
.Lcst_begin3:
.Lcst_end3:
                                        # -- End function
md::detail::sum_forcefield::~sum_forcefield():      # @md::detail::sum_forcefield::~sum_forcefield()
# %bb.0:
	push	r15
	push	r14
	push	r12
	push	rbx
	push	rax
	mov	qword ptr [rdi], offset vtable for md::detail::sum_forcefield+16
	mov	r15, qword ptr [rdi + 8]
	test	r15, r15
	je	.LBB10_9
# %bb.1:
	mov	r14, rdi
	mov	r12, qword ptr [rdi + 16]
	cmp	r12, r15
	je	.LBB10_2
.LBB10_3:                               # =>This Inner Loop Header: Depth=1
	mov	rbx, qword ptr [r12 - 8]
	add	r12, -16
	test	rbx, rbx
	je	.LBB10_6
# %bb.4:                                #   in Loop: Header=BB10_3 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	je	.LBB10_5
.LBB10_6:                               #   in Loop: Header=BB10_3 Depth=1
	cmp	r15, r12
	jne	.LBB10_3
	jmp	.LBB10_7
.LBB10_5:                               #   in Loop: Header=BB10_3 Depth=1
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
	cmp	r15, r12
	jne	.LBB10_3
.LBB10_7:
	mov	rdi, qword ptr [r14 + 8]
	jmp	.LBB10_8
.LBB10_9:
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	ret
.LBB10_2:
	mov	rdi, r15
.LBB10_8:
	mov	qword ptr [r14 + 16], r15
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end10:
                                        # -- End function
std::__1::__throw_length_error(char const*):    # @std::__1::__throw_length_error(char const*)
.Lfunc_begin4:
# %bb.0:
	push	r14
	push	rbx
	push	rax
	mov	r14, rdi
	mov	edi, 16
	call	__cxa_allocate_exception
	mov	rbx, rax
.Ltmp28:
	mov	rdi, rax
	mov	rsi, r14
	call	std::length_error::length_error(char const*)
.Ltmp29:
# %bb.1:
	mov	esi, offset typeinfo for std::length_error
	mov	edx, offset std::length_error::~length_error()
	mov	rdi, rbx
	call	__cxa_throw
.LBB11_2:
.Ltmp30:
	mov	r14, rax
	mov	rdi, rbx
	call	__cxa_free_exception
	mov	rdi, r14
	call	_Unwind_Resume
.Lfunc_end11:
GCC_except_table11:
.Lexception4:
.Lcst_begin4:
.Lcst_end4:
                                        # -- End function
std::length_error::length_error(char const*):              # @std::length_error::length_error(char const*)
# %bb.0:
	push	rbx
	mov	rbx, rdi
	call	std::logic_error::logic_error(char const*)
	mov	qword ptr [rbx], offset vtable for std::length_error+16
	pop	rbx
	ret
.Lfunc_end12:
                                        # -- End function
md::detail::sum_forcefield::~sum_forcefield():      # @md::detail::sum_forcefield::~sum_forcefield()
# %bb.0:
	push	r15
	push	r14
	push	r12
	push	rbx
	push	rax
	mov	r14, rdi
	mov	qword ptr [rdi], offset vtable for md::detail::sum_forcefield+16
	mov	r15, qword ptr [rdi + 8]
	test	r15, r15
	je	.LBB13_9
# %bb.1:
	mov	r12, qword ptr [r14 + 16]
	cmp	r12, r15
	je	.LBB13_2
.LBB13_3:                               # =>This Inner Loop Header: Depth=1
	mov	rbx, qword ptr [r12 - 8]
	add	r12, -16
	test	rbx, rbx
	je	.LBB13_6
# %bb.4:                                #   in Loop: Header=BB13_3 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	je	.LBB13_5
.LBB13_6:                               #   in Loop: Header=BB13_3 Depth=1
	cmp	r15, r12
	jne	.LBB13_3
	jmp	.LBB13_7
.LBB13_5:                               #   in Loop: Header=BB13_3 Depth=1
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
	cmp	r15, r12
	jne	.LBB13_3
.LBB13_7:
	mov	rdi, qword ptr [r14 + 8]
	jmp	.LBB13_8
.LBB13_2:
	mov	rdi, r15
.LBB13_8:
	mov	qword ptr [r14 + 16], r15
	call	operator delete(void*)
.LBB13_9:
	mov	rdi, r14
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end13:
                                        # -- End function
md::detail::sum_forcefield::compute_energy(md::system const&): # @md::detail::sum_forcefield::compute_energy(md::system const&)
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 16
	mov	rbx, qword ptr [rdi + 8]
	mov	r15, qword ptr [rdi + 16]
	cmp	rbx, r15
	je	.LBB14_1
# %bb.3:
	mov	r14, rsi
	xorpd	xmm0, xmm0
.LBB14_4:                               # =>This Inner Loop Header: Depth=1
	movsd	qword ptr [rsp + 8], xmm0 # 8-byte Spill
	mov	rdi, qword ptr [rbx]
	mov	rax, qword ptr [rdi]
	mov	rsi, r14
	call	qword ptr [rax + 16]
	movsd	xmm1, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm1 = mem[0],zero
	addsd	xmm1, xmm0
	movsd	qword ptr [rsp + 8], xmm1 # 8-byte Spill
	movsd	xmm0, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm0 = mem[0],zero
	add	rbx, 16
	cmp	r15, rbx
	jne	.LBB14_4
	jmp	.LBB14_2
.LBB14_1:
	xorpd	xmm0, xmm0
.LBB14_2:
	add	rsp, 16
	pop	rbx
	pop	r14
	pop	r15
	ret
.Lfunc_end14:
                                        # -- End function
md::detail::sum_forcefield::compute_force(md::system const&, md::array_view<md::vector>): # @md::detail::sum_forcefield::compute_force(md::system const&, md::array_view<md::vector>)
# %bb.0:
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 16
	mov	rbx, qword ptr [rdi + 8]
	mov	r13, qword ptr [rdi + 16]
	cmp	rbx, r13
	je	.LBB15_3
# %bb.1:
	mov	r14, rdx
	mov	r12, rsi
	mov	r15, rsp
.LBB15_2:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rbx]
	movups	xmm0, xmmword ptr [r14]
	movaps	xmmword ptr [rsp], xmm0
	mov	rax, qword ptr [rdi]
	mov	rsi, r12
	mov	rdx, r15
	call	qword ptr [rax + 24]
	add	rbx, 16
	cmp	r13, rbx
	jne	.LBB15_2
.LBB15_3:
	add	rsp, 16
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
.Lfunc_end15:
                                        # -- End function
std::__1::unique_ptr<md::detail::array_erasure::instance<double>, std::__1::default_delete<md::detail::array_erasure::instance<double> > > md::detail::array_erasure::make<double>(unsigned long, double): # @std::__1::unique_ptr<md::detail::array_erasure::instance<double>, std::__1::default_delete<md::detail::array_erasure::instance<double> > > md::detail::array_erasure::make<double>(unsigned long, double)
.Lfunc_begin5:
# %bb.0:
	push	r15
	push	r14
	push	r12
	push	rbx
	sub	rsp, 24
	movaps	xmmword ptr [rsp], xmm0 # 16-byte Spill
	mov	rbx, rsi
	mov	r14, rdi
	mov	edi, 40
	call	operator new(unsigned long)
	movaps	xmm1, xmmword ptr [rsp] # 16-byte Reload
	mov	r15, rax
	mov	qword ptr [rax], offset vtable for md::detail::array_erasure::instance<double>+16
	xorps	xmm0, xmm0
	movups	xmmword ptr [rax + 8], xmm0
	mov	qword ptr [rax + 24], 0
	test	rbx, rbx
	je	.LBB16_17
# %bb.1:
	mov	r12, rax
	add	r12, 8
	mov	rax, rbx
	shr	rax, 61
	jne	.LBB16_2
# %bb.4:
	lea	rdi, [8*rbx]
.Ltmp31:
	call	operator new(unsigned long)
.Ltmp32:
# %bb.5:
	mov	qword ptr [r15 + 16], rax
	mov	qword ptr [r15 + 8], rax
	lea	rcx, [rax + 8*rbx]
	mov	qword ptr [r15 + 24], rcx
	cmp	rbx, 4
	movaps	xmm1, xmmword ptr [rsp] # 16-byte Reload
	jb	.LBB16_15
# %bb.6:
	mov	rcx, rbx
	and	rcx, -4
	movaps	xmm0, xmm1
	movlhps	xmm0, xmm1              # xmm0 = xmm0[0],xmm1[0]
	lea	rdi, [rcx - 4]
	mov	rsi, rdi
	shr	rsi, 2
	add	rsi, 1
	mov	edx, esi
	and	edx, 7
	cmp	rdi, 28
	jae	.LBB16_8
# %bb.7:
	xor	esi, esi
	test	rdx, rdx
	jne	.LBB16_11
	jmp	.LBB16_13
.LBB16_8:
	mov	edi, 1
	sub	rdi, rsi
	add	rdi, rdx
	add	rdi, -1
	xor	esi, esi
.LBB16_9:                               # =>This Inner Loop Header: Depth=1
	movups	xmmword ptr [rax + 8*rsi], xmm0
	movups	xmmword ptr [rax + 8*rsi + 16], xmm0
	movups	xmmword ptr [rax + 8*rsi + 32], xmm0
	movups	xmmword ptr [rax + 8*rsi + 48], xmm0
	movups	xmmword ptr [rax + 8*rsi + 64], xmm0
	movups	xmmword ptr [rax + 8*rsi + 80], xmm0
	movups	xmmword ptr [rax + 8*rsi + 96], xmm0
	movups	xmmword ptr [rax + 8*rsi + 112], xmm0
	movups	xmmword ptr [rax + 8*rsi + 128], xmm0
	movups	xmmword ptr [rax + 8*rsi + 144], xmm0
	movups	xmmword ptr [rax + 8*rsi + 160], xmm0
	movups	xmmword ptr [rax + 8*rsi + 176], xmm0
	movups	xmmword ptr [rax + 8*rsi + 192], xmm0
	movups	xmmword ptr [rax + 8*rsi + 208], xmm0
	movups	xmmword ptr [rax + 8*rsi + 224], xmm0
	movups	xmmword ptr [rax + 8*rsi + 240], xmm0
	add	rsi, 32
	add	rdi, 8
	jne	.LBB16_9
# %bb.10:
	test	rdx, rdx
	je	.LBB16_13
.LBB16_11:
	lea	rsi, [rax + 8*rsi]
	add	rsi, 16
	neg	rdx
.LBB16_12:                              # =>This Inner Loop Header: Depth=1
	movups	xmmword ptr [rsi - 16], xmm0
	movups	xmmword ptr [rsi], xmm0
	add	rsi, 32
	inc	rdx
	jne	.LBB16_12
.LBB16_13:
	lea	rax, [rax + 8*rcx]
	cmp	rcx, rbx
	je	.LBB16_16
# %bb.14:
	sub	rbx, rcx
.LBB16_15:                              # =>This Inner Loop Header: Depth=1
	movsd	qword ptr [rax], xmm1
	add	rax, 8
	add	rbx, -1
	jne	.LBB16_15
.LBB16_16:
	mov	qword ptr [r15 + 16], rax
.LBB16_17:
	movsd	qword ptr [r15 + 32], xmm1
	mov	qword ptr [r14], r15
	mov	rax, r14
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	ret
.LBB16_2:
.Ltmp33:
	mov	rdi, r12
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Ltmp34:
# %bb.3:
.LBB16_18:
.Ltmp35:
	mov	rbx, rax
	mov	rdi, qword ptr [r12]
	test	rdi, rdi
	je	.LBB16_20
# %bb.19:
	mov	qword ptr [r15 + 16], rdi
	call	operator delete(void*)
.LBB16_20:
	mov	rdi, r15
	call	operator delete(void*)
	mov	rdi, rbx
	call	_Unwind_Resume
.Lfunc_end16:
GCC_except_table16:
.Lexception5:
.Lcst_begin5:
.Lcst_end5:
                                        # -- End function
md::detail::array_erasure::instance<double>::~instance(): # @md::detail::array_erasure::instance<double>::~instance()
# %bb.0:
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<double>+16
	mov	rax, qword ptr [rdi + 8]
	test	rax, rax
	je	.LBB17_1
# %bb.2:
	mov	qword ptr [rdi + 16], rax
	mov	rdi, rax
	jmp	operator delete(void*)                  # TAILCALL
.LBB17_1:
	ret
.Lfunc_end17:
                                        # -- End function
md::detail::array_erasure::instance<double>::~instance(): # @md::detail::array_erasure::instance<double>::~instance()
# %bb.0:
	push	rbx
	mov	rbx, rdi
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<double>+16
	mov	rdi, qword ptr [rdi + 8]
	test	rdi, rdi
	je	.LBB18_2
# %bb.1:
	mov	qword ptr [rbx + 16], rdi
	call	operator delete(void*)
.LBB18_2:
	mov	rdi, rbx
	pop	rbx
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end18:
                                        # -- End function
md::detail::array_erasure::instance<double>::size() const: # @md::detail::array_erasure::instance<double>::size() const
# %bb.0:
	mov	rax, qword ptr [rdi + 16]
	sub	rax, qword ptr [rdi + 8]
	sar	rax, 3
	ret
.Lfunc_end19:
                                        # -- End function
md::detail::array_erasure::instance<double>::resize(unsigned long): # @md::detail::array_erasure::instance<double>::resize(unsigned long)
# %bb.0:
	mov	rdx, rdi
	mov	rax, qword ptr [rdi + 8]
	mov	rcx, qword ptr [rdi + 16]
	sub	rcx, rax
	sar	rcx, 3
	cmp	rcx, rsi
	jae	.LBB20_1
# %bb.4:
	lea	rdi, [rdx + 8]
	add	rdx, 32
	sub	rsi, rcx
	jmp	std::__1::vector<double, std::__1::allocator<double> >::__append(unsigned long, double const&) # TAILCALL
.LBB20_1:
	jbe	.LBB20_3
# %bb.2:
	lea	rax, [rax + 8*rsi]
	mov	qword ptr [rdx + 16], rax
.LBB20_3:
	ret
.Lfunc_end20:
                                        # -- End function
std::__1::vector<double, std::__1::allocator<double> >::__append(unsigned long, double const&): # @std::__1::vector<double, std::__1::allocator<double> >::__append(unsigned long, double const&)
# %bb.0:
	push	rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 24
	mov	r15, rdx
	mov	r12, rsi
	mov	rbx, qword ptr [rdi + 8]
	mov	r13, qword ptr [rdi + 16]
	mov	rax, r13
	sub	rax, rbx
	sar	rax, 3
	cmp	rax, rsi
	jae	.LBB21_1
# %bb.19:
	mov	r8, qword ptr [rdi]
	sub	rbx, r8
	mov	r14, rbx
	sar	r14, 3
	lea	rsi, [r14 + r12]
	mov	rax, rsi
	shr	rax, 61
	jne	.LBB21_47
# %bb.20:
	movabs	rax, 2305843009213693951
	sub	r13, r8
	mov	rcx, r13
	sar	rcx, 3
	movabs	rdx, 1152921504606846974
	cmp	rcx, rdx
	mov	qword ptr [rsp + 16], rdi # 8-byte Spill
	mov	qword ptr [rsp + 8], rsi # 8-byte Spill
	ja	.LBB21_21
# %bb.22:
	sar	r13, 2
	cmp	r13, rsi
	cmovb	r13, rsi
	test	r13, r13
	je	.LBB21_23
# %bb.24:
	mov	rbp, r8
	cmp	r13, rax
	jbe	.LBB21_25
# %bb.48:
	mov	edi, offset .L.str
	call	std::__1::__throw_length_error(char const*)
.LBB21_1:
	cmp	r12, 4
	jb	.LBB21_12
# %bb.2:
	lea	rax, [r15 + 1]
	cmp	rax, rbx
	jbe	.LBB21_4
# %bb.3:
	lea	rax, [rbx + 8*r12]
	cmp	rax, r15
	ja	.LBB21_12
.LBB21_4:
	mov	rax, r12
	and	rax, -4
	lea	rsi, [rax - 4]
	mov	rdx, rsi
	shr	rdx, 2
	add	rdx, 1
	mov	ecx, edx
	and	ecx, 1
	test	rsi, rsi
	je	.LBB21_5
# %bb.6:
	movq	xmm0, qword ptr [r15]   # xmm0 = mem[0],zero
	pshufd	xmm0, xmm0, 68          # xmm0 = xmm0[0,1,0,1]
	mov	esi, 1
	sub	rsi, rdx
	add	rsi, rcx
	add	rsi, -1
	xor	edx, edx
.LBB21_7:                               # =>This Inner Loop Header: Depth=1
	movdqu	xmmword ptr [rbx + 8*rdx], xmm0
	movdqu	xmmword ptr [rbx + 8*rdx + 16], xmm0
	movdqu	xmmword ptr [rbx + 8*rdx + 32], xmm0
	movdqu	xmmword ptr [rbx + 8*rdx + 48], xmm0
	add	rdx, 8
	add	rsi, 2
	jne	.LBB21_7
# %bb.8:
	test	rcx, rcx
	je	.LBB21_10
.LBB21_9:
	movq	xmm0, qword ptr [r15]   # xmm0 = mem[0],zero
	pshufd	xmm0, xmm0, 68          # xmm0 = xmm0[0,1,0,1]
	movdqu	xmmword ptr [rbx + 8*rdx], xmm0
	movdqu	xmmword ptr [rbx + 8*rdx + 16], xmm0
.LBB21_10:
	lea	rbx, [rbx + 8*rax]
	cmp	rax, r12
	je	.LBB21_18
# %bb.11:
	sub	r12, rax
.LBB21_12:
	lea	rax, [r12 - 1]
	mov	rcx, r12
	and	rcx, 7
	je	.LBB21_16
# %bb.13:
	xor	edx, edx
.LBB21_14:                              # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r15]
	mov	qword ptr [rbx], rsi
	add	rbx, 8
	add	rdx, 1
	cmp	rcx, rdx
	jne	.LBB21_14
# %bb.15:
	sub	r12, rdx
.LBB21_16:
	cmp	rax, 7
	jb	.LBB21_18
.LBB21_17:                              # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 8], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 16], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 24], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 32], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 40], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 48], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbx + 56], rax
	add	rbx, 64
	add	r12, -8
	jne	.LBB21_17
.LBB21_18:
	mov	qword ptr [rdi + 8], rbx
	jmp	.LBB21_46
.LBB21_21:
	mov	rbp, r8
	mov	r13, rax
.LBB21_25:
	lea	rdi, [8*r13]
	call	operator new(unsigned long)
	mov	r9, rax
	mov	r8, rbp
.LBB21_26:
	lea	rbp, [r9 + 8*r14]
	cmp	r12, 4
	jb	.LBB21_37
# %bb.27:
	lea	rax, [r15 + 1]
	cmp	rax, rbp
	jbe	.LBB21_29
# %bb.28:
	mov	rax, qword ptr [rsp + 8] # 8-byte Reload
	lea	rax, [r9 + 8*rax]
	cmp	rax, r15
	ja	.LBB21_37
.LBB21_29:
	mov	rax, r12
	and	rax, -4
	lea	rsi, [rax - 4]
	mov	rdx, rsi
	shr	rdx, 2
	add	rdx, 1
	mov	ecx, edx
	and	ecx, 1
	test	rsi, rsi
	je	.LBB21_30
# %bb.31:
	movq	xmm0, qword ptr [r15]   # xmm0 = mem[0],zero
	pshufd	xmm0, xmm0, 68          # xmm0 = xmm0[0,1,0,1]
	lea	rsi, [r9 + 8*r14]
	add	rsi, 48
	mov	edi, 1
	sub	rdi, rdx
	add	rdi, rcx
	add	rdi, -1
	xor	edx, edx
.LBB21_32:                              # =>This Inner Loop Header: Depth=1
	movdqu	xmmword ptr [rsi + 8*rdx - 48], xmm0
	movdqu	xmmword ptr [rsi + 8*rdx - 32], xmm0
	movdqu	xmmword ptr [rsi + 8*rdx - 16], xmm0
	movdqu	xmmword ptr [rsi + 8*rdx], xmm0
	add	rdx, 8
	add	rdi, 2
	jne	.LBB21_32
# %bb.33:
	test	rcx, rcx
	je	.LBB21_35
.LBB21_34:
	movq	xmm0, qword ptr [r15]   # xmm0 = mem[0],zero
	pshufd	xmm0, xmm0, 68          # xmm0 = xmm0[0,1,0,1]
	movdqu	xmmword ptr [rbp + 8*rdx], xmm0
	movdqu	xmmword ptr [rbp + 8*rdx + 16], xmm0
.LBB21_35:
	lea	rbp, [rbp + 8*rax]
	cmp	rax, r12
	je	.LBB21_43
# %bb.36:
	sub	r12, rax
.LBB21_37:
	lea	rax, [r12 - 1]
	mov	rcx, r12
	and	rcx, 7
	je	.LBB21_41
# %bb.38:
	xor	edx, edx
.LBB21_39:                              # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r15]
	mov	qword ptr [rbp], rsi
	add	rbp, 8
	add	rdx, 1
	cmp	rcx, rdx
	jne	.LBB21_39
# %bb.40:
	sub	r12, rdx
.LBB21_41:
	cmp	rax, 7
	jb	.LBB21_43
.LBB21_42:                              # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 8], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 16], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 24], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 32], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 40], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 48], rax
	mov	rax, qword ptr [r15]
	mov	qword ptr [rbp + 56], rax
	add	rbp, 64
	add	r12, -8
	jne	.LBB21_42
.LBB21_43:
	lea	r14, [r9 + 8*r13]
	test	rbx, rbx
	jle	.LBB21_45
# %bb.44:
	mov	rdi, r9
	mov	rsi, r8
	mov	rdx, rbx
	mov	r15, r8
	mov	rbx, r9
	call	memcpy
	mov	r9, rbx
	mov	r8, r15
.LBB21_45:
	mov	rax, qword ptr [rsp + 16] # 8-byte Reload
	mov	qword ptr [rax], r9
	mov	qword ptr [rax + 8], rbp
	mov	qword ptr [rax + 16], r14
	test	r8, r8
	je	.LBB21_46
# %bb.49:
	mov	rdi, r8
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	jmp	operator delete(void*)                  # TAILCALL
.LBB21_46:
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
.LBB21_23:
	xor	r13d, r13d
	xor	r9d, r9d
	jmp	.LBB21_26
.LBB21_5:
	xor	edx, edx
	test	rcx, rcx
	jne	.LBB21_9
	jmp	.LBB21_10
.LBB21_30:
	xor	edx, edx
	test	rcx, rcx
	jne	.LBB21_34
	jmp	.LBB21_35
.LBB21_47:
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Lfunc_end21:
                                        # -- End function
std::__1::unique_ptr<md::detail::array_erasure::instance<md::point>, std::__1::default_delete<md::detail::array_erasure::instance<md::point> > > md::detail::array_erasure::make<md::point>(unsigned long, md::point): # @std::__1::unique_ptr<md::detail::array_erasure::instance<md::point>, std::__1::default_delete<md::detail::array_erasure::instance<md::point> > > md::detail::array_erasure::make<md::point>(unsigned long, md::point)
.Lfunc_begin6:
# %bb.0:
	push	r15
	push	r14
	push	r12
	push	rbx
	sub	rsp, 24
	mov	r15, rsi
	mov	r14, rdi
	mov	edi, 56
	call	operator new(unsigned long)
	mov	r12, rax
	mov	rbx, rax
	mov	rax, qword ptr [rsp + 80]
	mov	qword ptr [rsp + 16], rax
	movaps	xmm0, xmmword ptr [rsp + 64]
	movaps	xmmword ptr [rsp], xmm0
	mov	qword ptr [r12], offset vtable for md::detail::array_erasure::instance<md::point>+16
	xorps	xmm0, xmm0
	movups	xmmword ptr [r12 + 8], xmm0
	mov	qword ptr [r12 + 24], 0
	test	r15, r15
	je	.LBB22_11
# %bb.1:
	add	r12, 8
	movabs	rax, 768614336404564651
	cmp	r15, rax
	jae	.LBB22_2
# %bb.4:
	lea	rax, [8*r15]
	lea	rdi, [rax + 2*rax]
.Ltmp36:
	call	operator new(unsigned long)
.Ltmp37:
# %bb.5:
	mov	rcx, rax
	mov	qword ptr [rbx + 16], rax
	mov	qword ptr [rbx + 8], rax
	lea	rax, [r15 + 2*r15]
	lea	rax, [rcx + 8*rax]
	mov	qword ptr [rbx + 24], rax
	lea	rax, [r15 - 1]
	mov	rsi, r15
	and	rsi, 3
	je	.LBB22_9
# %bb.6:
	xor	edx, edx
.LBB22_7:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rsp + 16]
	mov	qword ptr [rcx + 16], rdi
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rcx], xmm0
	mov	rcx, qword ptr [rbx + 16]
	add	rcx, 24
	mov	qword ptr [rbx + 16], rcx
	add	rdx, 1
	cmp	rsi, rdx
	jne	.LBB22_7
# %bb.8:
	sub	r15, rdx
.LBB22_9:
	cmp	rax, 3
	jb	.LBB22_11
.LBB22_10:                              # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rsp + 16]
	mov	qword ptr [rcx + 16], rax
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rcx], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rcx, qword ptr [rbx + 16]
	add	rcx, 24
	mov	qword ptr [rbx + 16], rcx
	add	r15, -4
	jne	.LBB22_10
.LBB22_11:
	mov	rax, qword ptr [rsp + 16]
	mov	qword ptr [rbx + 48], rax
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rbx + 32], xmm0
	mov	qword ptr [r14], rbx
	mov	rax, r14
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	ret
.LBB22_2:
.Ltmp38:
	mov	rdi, r12
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Ltmp39:
# %bb.3:
.LBB22_12:
.Ltmp40:
	mov	r14, rax
	mov	rdi, qword ptr [r12]
	test	rdi, rdi
	je	.LBB22_14
# %bb.13:
	mov	qword ptr [rbx + 16], rdi
	call	operator delete(void*)
.LBB22_14:
	mov	rdi, rbx
	call	operator delete(void*)
	mov	rdi, r14
	call	_Unwind_Resume
.Lfunc_end22:
GCC_except_table22:
.Lexception6:
.Lcst_begin6:
.Lcst_end6:
                                        # -- End function
md::detail::array_erasure::instance<md::point>::~instance(): # @md::detail::array_erasure::instance<md::point>::~instance()
# %bb.0:
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<md::point>+16
	mov	rax, qword ptr [rdi + 8]
	test	rax, rax
	je	.LBB23_1
# %bb.2:
	mov	qword ptr [rdi + 16], rax
	mov	rdi, rax
	jmp	operator delete(void*)                  # TAILCALL
.LBB23_1:
	ret
.Lfunc_end23:
                                        # -- End function
md::detail::array_erasure::instance<md::point>::~instance(): # @md::detail::array_erasure::instance<md::point>::~instance()
# %bb.0:
	push	rbx
	mov	rbx, rdi
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<md::point>+16
	mov	rdi, qword ptr [rdi + 8]
	test	rdi, rdi
	je	.LBB24_2
# %bb.1:
	mov	qword ptr [rbx + 16], rdi
	call	operator delete(void*)
.LBB24_2:
	mov	rdi, rbx
	pop	rbx
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end24:
                                        # -- End function
md::detail::array_erasure::instance<md::point>::size() const: # @md::detail::array_erasure::instance<md::point>::size() const
# %bb.0:
	mov	rcx, qword ptr [rdi + 16]
	sub	rcx, qword ptr [rdi + 8]
	sar	rcx, 3
	movabs	rax, -6148914691236517205
	imul	rax, rcx
	ret
.Lfunc_end25:
                                        # -- End function
md::detail::array_erasure::instance<md::point>::resize(unsigned long): # @md::detail::array_erasure::instance<md::point>::resize(unsigned long)
# %bb.0:
	mov	rdx, rdi
	mov	rax, qword ptr [rdi + 8]
	mov	rdi, qword ptr [rdi + 16]
	sub	rdi, rax
	sar	rdi, 3
	movabs	rcx, -6148914691236517205
	imul	rcx, rdi
	cmp	rcx, rsi
	jae	.LBB26_1
# %bb.4:
	lea	rdi, [rdx + 8]
	add	rdx, 32
	sub	rsi, rcx
	jmp	std::__1::vector<md::point, std::__1::allocator<md::point> >::__append(unsigned long, md::point const&) # TAILCALL
.LBB26_1:
	jbe	.LBB26_3
# %bb.2:
	lea	rcx, [rsi + 2*rsi]
	lea	rax, [rax + 8*rcx]
	mov	qword ptr [rdx + 16], rax
.LBB26_3:
	ret
.Lfunc_end26:
                                        # -- End function
std::__1::vector<md::point, std::__1::allocator<md::point> >::__append(unsigned long, md::point const&): # @std::__1::vector<md::point, std::__1::allocator<md::point> >::__append(unsigned long, md::point const&)
# %bb.0:
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	mov	r12, rdx
	mov	r15, rsi
	mov	r14, rdi
	mov	rbx, qword ptr [rdi + 8]
	mov	rax, qword ptr [rdi + 16]
	mov	rdx, rax
	sub	rdx, rbx
	sar	rdx, 3
	movabs	rcx, -6148914691236517205
	imul	rdx, rcx
	cmp	rdx, rsi
	jae	.LBB27_1
# %bb.7:
	movabs	r13, 768614336404564650
	mov	rsi, qword ptr [r14]
	sub	rbx, rsi
	sar	rbx, 3
	imul	rbx, rcx
	lea	rdx, [rbx + r15]
	cmp	rdx, r13
	ja	.LBB27_23
# %bb.8:
	sub	rax, rsi
	sar	rax, 3
	imul	rax, rcx
	movabs	rcx, 384307168202282324
	cmp	rax, rcx
	ja	.LBB27_11
# %bb.9:
	add	rax, rax
	cmp	rax, rdx
	cmovb	rax, rdx
	mov	r13, rax
	test	rax, rax
	je	.LBB27_10
.LBB27_11:
	lea	rax, [8*r13]
	lea	rdi, [rax + 2*rax]
	call	operator new(unsigned long)
.LBB27_12:
	lea	rcx, [rbx + 2*rbx]
	lea	r8, [rax + 8*rcx]
	lea	rdx, [r15 - 1]
	mov	rcx, r15
	and	rcx, 7
	je	.LBB27_13
# %bb.14:
	xor	edi, edi
	mov	rbx, r8
.LBB27_15:                              # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rsi
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	add	rbx, 24
	add	rdi, 1
	cmp	rcx, rdi
	jne	.LBB27_15
# %bb.16:
	sub	r15, rdi
	jmp	.LBB27_17
.LBB27_1:
	lea	rax, [r15 - 1]
	mov	rcx, r15
	and	rcx, 3
	je	.LBB27_5
# %bb.2:
	xor	edx, edx
.LBB27_3:                               # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rsi
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rbx, qword ptr [r14 + 8]
	add	rbx, 24
	mov	qword ptr [r14 + 8], rbx
	add	rdx, 1
	cmp	rcx, rdx
	jne	.LBB27_3
# %bb.4:
	sub	r15, rdx
.LBB27_5:
	cmp	rax, 3
	jb	.LBB27_22
.LBB27_6:                               # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rax
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rbx, qword ptr [r14 + 8]
	add	rbx, 24
	mov	qword ptr [r14 + 8], rbx
	add	r15, -4
	jne	.LBB27_6
	jmp	.LBB27_22
.LBB27_13:
	mov	rbx, r8
.LBB27_17:
	lea	rcx, [2*r13]
	add	rcx, r13
	cmp	rdx, 7
	jb	.LBB27_19
.LBB27_18:                              # =>This Inner Loop Header: Depth=1
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 40], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 24], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 64], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 48], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 88], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 72], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 112], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 96], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 136], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 120], xmm0
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 144], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 160], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 168], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 184], rdx
	add	rbx, 192
	add	r15, -8
	jne	.LBB27_18
.LBB27_19:
	lea	r13, [rax + 8*rcx]
	mov	r12, qword ptr [r14]
	mov	rcx, qword ptr [r14 + 8]
	sub	rcx, r12
	movabs	rdx, -3074457345618258603
	mov	rax, rcx
	imul	rdx
	mov	rax, rdx
	shr	rax, 63
	sar	rdx, 2
	add	rdx, rax
	lea	rax, [rdx + 2*rdx]
	lea	r15, [r8 + 8*rax]
	test	rcx, rcx
	jle	.LBB27_21
# %bb.20:
	mov	rdi, r15
	mov	rsi, r12
	mov	rdx, rcx
	call	memcpy
.LBB27_21:
	mov	qword ptr [r14], r15
	mov	qword ptr [r14 + 8], rbx
	mov	qword ptr [r14 + 16], r13
	test	r12, r12
	je	.LBB27_22
# %bb.24:
	mov	rdi, r12
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	jmp	operator delete(void*)                  # TAILCALL
.LBB27_22:
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
.LBB27_10:
	xor	r13d, r13d
	xor	eax, eax
	jmp	.LBB27_12
.LBB27_23:
	mov	rdi, r14
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Lfunc_end27:
                                        # -- End function
std::__1::unique_ptr<md::detail::array_erasure::instance<md::vector>, std::__1::default_delete<md::detail::array_erasure::instance<md::vector> > > md::detail::array_erasure::make<md::vector>(unsigned long, md::vector): # @std::__1::unique_ptr<md::detail::array_erasure::instance<md::vector>, std::__1::default_delete<md::detail::array_erasure::instance<md::vector> > > md::detail::array_erasure::make<md::vector>(unsigned long, md::vector)
.Lfunc_begin7:
# %bb.0:
	push	r15
	push	r14
	push	r12
	push	rbx
	sub	rsp, 24
	mov	r15, rsi
	mov	r14, rdi
	mov	edi, 56
	call	operator new(unsigned long)
	mov	r12, rax
	mov	rbx, rax
	mov	rax, qword ptr [rsp + 80]
	mov	qword ptr [rsp + 16], rax
	movaps	xmm0, xmmword ptr [rsp + 64]
	movaps	xmmword ptr [rsp], xmm0
	mov	qword ptr [r12], offset vtable for md::detail::array_erasure::instance<md::vector>+16
	xorps	xmm0, xmm0
	movups	xmmword ptr [r12 + 8], xmm0
	mov	qword ptr [r12 + 24], 0
	test	r15, r15
	je	.LBB28_11
# %bb.1:
	add	r12, 8
	movabs	rax, 768614336404564651
	cmp	r15, rax
	jae	.LBB28_2
# %bb.4:
	lea	rax, [8*r15]
	lea	rdi, [rax + 2*rax]
.Ltmp41:
	call	operator new(unsigned long)
.Ltmp42:
# %bb.5:
	mov	rcx, rax
	mov	qword ptr [rbx + 16], rax
	mov	qword ptr [rbx + 8], rax
	lea	rax, [r15 + 2*r15]
	lea	rax, [rcx + 8*rax]
	mov	qword ptr [rbx + 24], rax
	lea	rax, [r15 - 1]
	mov	rsi, r15
	and	rsi, 3
	je	.LBB28_9
# %bb.6:
	xor	edx, edx
.LBB28_7:                               # =>This Inner Loop Header: Depth=1
	mov	rdi, qword ptr [rsp + 16]
	mov	qword ptr [rcx + 16], rdi
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rcx], xmm0
	mov	rcx, qword ptr [rbx + 16]
	add	rcx, 24
	mov	qword ptr [rbx + 16], rcx
	add	rdx, 1
	cmp	rsi, rdx
	jne	.LBB28_7
# %bb.8:
	sub	r15, rdx
.LBB28_9:
	cmp	rax, 3
	jb	.LBB28_11
.LBB28_10:                              # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [rsp + 16]
	mov	qword ptr [rcx + 16], rax
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rcx], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [rbx + 16]
	lea	rcx, [rax + 24]
	mov	qword ptr [rbx + 16], rcx
	mov	rcx, qword ptr [rsp + 16]
	mov	qword ptr [rax + 40], rcx
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rcx, qword ptr [rbx + 16]
	add	rcx, 24
	mov	qword ptr [rbx + 16], rcx
	add	r15, -4
	jne	.LBB28_10
.LBB28_11:
	mov	rax, qword ptr [rsp + 16]
	mov	qword ptr [rbx + 48], rax
	movaps	xmm0, xmmword ptr [rsp]
	movups	xmmword ptr [rbx + 32], xmm0
	mov	qword ptr [r14], rbx
	mov	rax, r14
	add	rsp, 24
	pop	rbx
	pop	r12
	pop	r14
	pop	r15
	ret
.LBB28_2:
.Ltmp43:
	mov	rdi, r12
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Ltmp44:
# %bb.3:
.LBB28_12:
.Ltmp45:
	mov	r14, rax
	mov	rdi, qword ptr [r12]
	test	rdi, rdi
	je	.LBB28_14
# %bb.13:
	mov	qword ptr [rbx + 16], rdi
	call	operator delete(void*)
.LBB28_14:
	mov	rdi, rbx
	call	operator delete(void*)
	mov	rdi, r14
	call	_Unwind_Resume
.Lfunc_end28:
GCC_except_table28:
.Lexception7:
.Lcst_begin7:
.Lcst_end7:
                                        # -- End function
md::detail::array_erasure::instance<md::vector>::~instance(): # @md::detail::array_erasure::instance<md::vector>::~instance()
# %bb.0:
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<md::vector>+16
	mov	rax, qword ptr [rdi + 8]
	test	rax, rax
	je	.LBB29_1
# %bb.2:
	mov	qword ptr [rdi + 16], rax
	mov	rdi, rax
	jmp	operator delete(void*)                  # TAILCALL
.LBB29_1:
	ret
.Lfunc_end29:
                                        # -- End function
md::detail::array_erasure::instance<md::vector>::~instance(): # @md::detail::array_erasure::instance<md::vector>::~instance()
# %bb.0:
	push	rbx
	mov	rbx, rdi
	mov	qword ptr [rdi], offset vtable for md::detail::array_erasure::instance<md::vector>+16
	mov	rdi, qword ptr [rdi + 8]
	test	rdi, rdi
	je	.LBB30_2
# %bb.1:
	mov	qword ptr [rbx + 16], rdi
	call	operator delete(void*)
.LBB30_2:
	mov	rdi, rbx
	pop	rbx
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end30:
                                        # -- End function
md::detail::array_erasure::instance<md::vector>::size() const: # @md::detail::array_erasure::instance<md::vector>::size() const
# %bb.0:
	mov	rcx, qword ptr [rdi + 16]
	sub	rcx, qword ptr [rdi + 8]
	sar	rcx, 3
	movabs	rax, -6148914691236517205
	imul	rax, rcx
	ret
.Lfunc_end31:
                                        # -- End function
md::detail::array_erasure::instance<md::vector>::resize(unsigned long): # @md::detail::array_erasure::instance<md::vector>::resize(unsigned long)
# %bb.0:
	mov	rdx, rdi
	mov	rax, qword ptr [rdi + 8]
	mov	rdi, qword ptr [rdi + 16]
	sub	rdi, rax
	sar	rdi, 3
	movabs	rcx, -6148914691236517205
	imul	rcx, rdi
	cmp	rcx, rsi
	jae	.LBB32_1
# %bb.4:
	lea	rdi, [rdx + 8]
	add	rdx, 32
	sub	rsi, rcx
	jmp	std::__1::vector<md::vector, std::__1::allocator<md::vector> >::__append(unsigned long, md::vector const&) # TAILCALL
.LBB32_1:
	jbe	.LBB32_3
# %bb.2:
	lea	rcx, [rsi + 2*rsi]
	lea	rax, [rax + 8*rcx]
	mov	qword ptr [rdx + 16], rax
.LBB32_3:
	ret
.Lfunc_end32:
                                        # -- End function
std::__1::vector<md::vector, std::__1::allocator<md::vector> >::__append(unsigned long, md::vector const&): # @std::__1::vector<md::vector, std::__1::allocator<md::vector> >::__append(unsigned long, md::vector const&)
# %bb.0:
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	mov	r12, rdx
	mov	r15, rsi
	mov	r14, rdi
	mov	rbx, qword ptr [rdi + 8]
	mov	rax, qword ptr [rdi + 16]
	mov	rdx, rax
	sub	rdx, rbx
	sar	rdx, 3
	movabs	rcx, -6148914691236517205
	imul	rdx, rcx
	cmp	rdx, rsi
	jae	.LBB33_1
# %bb.7:
	movabs	r13, 768614336404564650
	mov	rsi, qword ptr [r14]
	sub	rbx, rsi
	sar	rbx, 3
	imul	rbx, rcx
	lea	rdx, [rbx + r15]
	cmp	rdx, r13
	ja	.LBB33_23
# %bb.8:
	sub	rax, rsi
	sar	rax, 3
	imul	rax, rcx
	movabs	rcx, 384307168202282324
	cmp	rax, rcx
	ja	.LBB33_11
# %bb.9:
	add	rax, rax
	cmp	rax, rdx
	cmovb	rax, rdx
	mov	r13, rax
	test	rax, rax
	je	.LBB33_10
.LBB33_11:
	lea	rax, [8*r13]
	lea	rdi, [rax + 2*rax]
	call	operator new(unsigned long)
.LBB33_12:
	lea	rcx, [rbx + 2*rbx]
	lea	r8, [rax + 8*rcx]
	lea	rdx, [r15 - 1]
	mov	rcx, r15
	and	rcx, 7
	je	.LBB33_13
# %bb.14:
	xor	edi, edi
	mov	rbx, r8
.LBB33_15:                              # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rsi
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	add	rbx, 24
	add	rdi, 1
	cmp	rcx, rdi
	jne	.LBB33_15
# %bb.16:
	sub	r15, rdi
	jmp	.LBB33_17
.LBB33_1:
	lea	rax, [r15 - 1]
	mov	rcx, r15
	and	rcx, 3
	je	.LBB33_5
# %bb.2:
	xor	edx, edx
.LBB33_3:                               # =>This Inner Loop Header: Depth=1
	mov	rsi, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rsi
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rbx, qword ptr [r14 + 8]
	add	rbx, 24
	mov	qword ptr [r14 + 8], rbx
	add	rdx, 1
	cmp	rcx, rdx
	jne	.LBB33_3
# %bb.4:
	sub	r15, rdx
.LBB33_5:
	cmp	rax, 3
	jb	.LBB33_22
.LBB33_6:                               # =>This Inner Loop Header: Depth=1
	mov	rax, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rax
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rax, qword ptr [r14 + 8]
	lea	rcx, [rax + 24]
	mov	qword ptr [r14 + 8], rcx
	mov	rcx, qword ptr [r12 + 16]
	mov	qword ptr [rax + 40], rcx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rax + 24], xmm0
	mov	rbx, qword ptr [r14 + 8]
	add	rbx, 24
	mov	qword ptr [r14 + 8], rbx
	add	r15, -4
	jne	.LBB33_6
	jmp	.LBB33_22
.LBB33_13:
	mov	rbx, r8
.LBB33_17:
	lea	rcx, [2*r13]
	add	rcx, r13
	cmp	rdx, 7
	jb	.LBB33_19
.LBB33_18:                              # =>This Inner Loop Header: Depth=1
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 16], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 40], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 24], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 64], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 48], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 88], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 72], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 112], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 96], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 136], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 120], xmm0
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 144], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 160], rdx
	movups	xmm0, xmmword ptr [r12]
	movups	xmmword ptr [rbx + 168], xmm0
	mov	rdx, qword ptr [r12 + 16]
	mov	qword ptr [rbx + 184], rdx
	add	rbx, 192
	add	r15, -8
	jne	.LBB33_18
.LBB33_19:
	lea	r13, [rax + 8*rcx]
	mov	r12, qword ptr [r14]
	mov	rcx, qword ptr [r14 + 8]
	sub	rcx, r12
	movabs	rdx, -3074457345618258603
	mov	rax, rcx
	imul	rdx
	mov	rax, rdx
	shr	rax, 63
	sar	rdx, 2
	add	rdx, rax
	lea	rax, [rdx + 2*rdx]
	lea	r15, [r8 + 8*rax]
	test	rcx, rcx
	jle	.LBB33_21
# %bb.20:
	mov	rdi, r15
	mov	rsi, r12
	mov	rdx, rcx
	call	memcpy
.LBB33_21:
	mov	qword ptr [r14], r15
	mov	qword ptr [r14 + 8], rbx
	mov	qword ptr [r14 + 16], r13
	test	r12, r12
	je	.LBB33_22
# %bb.24:
	mov	rdi, r12
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	jmp	operator delete(void*)                  # TAILCALL
.LBB33_22:
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
.LBB33_10:
	xor	r13d, r13d
	xor	eax, eax
	jmp	.LBB33_12
.LBB33_23:
	mov	rdi, r14
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Lfunc_end33:
                                        # -- End function
md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_energy(md::system const&): # @md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_energy(md::system const&)
# %bb.0:
	push	r14
	push	rbx
	push	rax
	mov	r14, rsi
	mov	rbx, rdi
	call	md::all_pair_forcefield<my_forcefield>::compute_energy(md::system const&)
	movsd	qword ptr [rsp], xmm0   # 8-byte Spill
	add	rbx, 8
	mov	rdi, rbx
	mov	rsi, r14
	call	md::sphere_surface_forcefield<my_forcefield>::compute_energy(md::system const&)
	xorpd	xmm1, xmm1
	movsd	xmm2, qword ptr [rsp]   # 8-byte Reload
                                        # xmm2 = mem[0],zero
	addsd	xmm2, xmm1
	addsd	xmm2, xmm0
	movapd	xmm0, xmm2
	add	rsp, 8
	pop	rbx
	pop	r14
	ret
.Lfunc_end34:
                                        # -- End function
md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_force(md::system const&, md::array_view<md::vector>): # @md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_force(md::system const&, md::array_view<md::vector>)
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 32
	mov	r15, rdx
	mov	r14, rsi
	mov	rbx, rdi
	movups	xmm0, xmmword ptr [rdx]
	movaps	xmmword ptr [rsp + 16], xmm0
	lea	rdx, [rsp + 16]
	call	md::all_pair_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
	add	rbx, 8
	movups	xmm0, xmmword ptr [r15]
	movaps	xmmword ptr [rsp], xmm0
	mov	rdx, rsp
	mov	rdi, rbx
	mov	rsi, r14
	call	md::sphere_surface_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
	add	rsp, 32
	pop	rbx
	pop	r14
	pop	r15
	ret
.Lfunc_end35:
                                        # -- End function
.LCPI36_0:
.LCPI36_1:
.LCPI36_2:
.LCPI36_3:
non-virtual thunk to md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_energy(md::system const&): # @non-virtual thunk to md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_energy(md::system const&)
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 96
	mov	r14, rsi
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	r8, qword ptr [rsi + 8]
	mov	rax, qword ptr [r8 + 8*rax]
	mov	r15, qword ptr [rax + 8]
	mov	rax, qword ptr [rax + 16]
	xorpd	xmm0, xmm0
	movsd	qword ptr [rsp], xmm0   # 8-byte Spill
	xorpd	xmm2, xmm2
	sub	rax, r15
	je	.LBB36_6
# %bb.1:
	sar	rax, 3
	movabs	rcx, -6148914691236517205
	imul	rcx, rax
	mov	r9, qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value]
	lea	r10, [r15 + 16]
	xorpd	xmm2, xmm2
	xor	ebx, ebx
	movsd	xmm8, qword ptr [rip + .LCPI36_0] # xmm8 = mem[0],zero
	movsd	xmm1, qword ptr [rip + .LCPI36_1] # xmm1 = mem[0],zero
	movsd	xmm9, qword ptr [rip + .LCPI36_2] # xmm9 = mem[0],zero
.LBB36_2:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB36_4 Depth 2
	test	rbx, rbx
	je	.LBB36_5
# %bb.3:                                #   in Loop: Header=BB36_2 Depth=1
	mov	rax, qword ptr [r8 + 8*r9]
	mov	rax, qword ptr [rax + 8]
	movsd	xmm3, qword ptr [rax + 8*rbx] # xmm3 = mem[0],zero
	lea	rdx, [rbx + 2*rbx]
	movsd	xmm4, qword ptr [r15 + 8*rdx] # xmm4 = mem[0],zero
	movsd	xmm5, qword ptr [r15 + 8*rdx + 8] # xmm5 = mem[0],zero
	movhpd	xmm4, qword ptr [r15 + 8*rdx + 16] # xmm4 = xmm4[0],mem[0]
	mov	rsi, r10
	xor	edx, edx
.LBB36_4:                               #   Parent Loop BB36_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movsd	xmm6, qword ptr [rax + 8*rdx] # xmm6 = mem[0],zero
	addsd	xmm6, xmm3
	movsd	xmm7, qword ptr [rsi - 16] # xmm7 = mem[0],zero
	movsd	xmm0, qword ptr [rsi - 8] # xmm0 = mem[0],zero
	subsd	xmm0, xmm5
	movhpd	xmm7, qword ptr [rsi]   # xmm7 = xmm7[0],mem[0]
	subpd	xmm7, xmm4
	mulsd	xmm0, xmm0
	mulpd	xmm7, xmm7
	addsd	xmm0, xmm7
	unpckhpd	xmm7, xmm7      # xmm7 = xmm7[1,1]
	addsd	xmm7, xmm0
	mulsd	xmm6, xmm6
	divsd	xmm7, xmm6
	movapd	xmm0, xmm7
	mulsd	xmm0, xmm7
	mulsd	xmm0, xmm7
	addsd	xmm0, xmm8
	movapd	xmm6, xmm1
	divsd	xmm6, xmm0
	addsd	xmm6, xmm9
	mulsd	xmm6, xmm6
	addsd	xmm2, xmm6
	add	rdx, 1
	add	rsi, 24
	cmp	rbx, rdx
	jne	.LBB36_4
.LBB36_5:                               #   in Loop: Header=BB36_2 Depth=1
	add	rbx, 1
	cmp	rbx, rcx
	jb	.LBB36_2
.LBB36_6:
	cmp	qword ptr [r14], 0
	je	.LBB36_16
# %bb.7:
	movsd	xmm8, qword ptr [rdi + 16] # xmm8 = mem[0],zero
	movupd	xmm9, xmmword ptr [rdi + 24]
	movsd	xmm0, qword ptr [rdi + 40] # xmm0 = mem[0],zero
	movsd	qword ptr [rsp + 8], xmm0 # 8-byte Spill
	mulsd	xmm0, xmm0
	movsd	qword ptr [rsp + 16], xmm0 # 8-byte Spill
	add	r15, 8
	xorpd	xmm7, xmm7
	xor	ebx, ebx
	xorpd	xmm0, xmm0
	movsd	qword ptr [rsp], xmm0   # 8-byte Spill
	movsd	qword ptr [rsp + 32], xmm2 # 8-byte Spill
	movsd	qword ptr [rsp + 24], xmm8 # 8-byte Spill
	movapd	xmmword ptr [rsp + 48], xmm9 # 16-byte Spill
.LBB36_8:                               # =>This Inner Loop Header: Depth=1
	movsd	xmm6, qword ptr [r15 - 8] # xmm6 = mem[0],zero
	subsd	xmm6, xmm8
	movupd	xmm1, xmmword ptr [r15]
	subpd	xmm1, xmm9
	movapd	xmm0, xmm6
	mulsd	xmm0, xmm6
	movapd	xmm4, xmm1
	mulpd	xmm4, xmm1
	addsd	xmm0, xmm4
	unpckhpd	xmm4, xmm4      # xmm4 = xmm4[1,1]
	addsd	xmm4, xmm0
	ucomisd	xmm4, xmm7
	jb	.LBB36_10
# %bb.9:                                #   in Loop: Header=BB36_8 Depth=1
	xorps	xmm0, xmm0
	sqrtsd	xmm0, xmm4
	jmp	.LBB36_11
.LBB36_10:                              #   in Loop: Header=BB36_8 Depth=1
	movapd	xmm0, xmm4
	movapd	xmmword ptr [rsp + 80], xmm1 # 16-byte Spill
	movsd	qword ptr [rsp + 40], xmm6 # 8-byte Spill
	movapd	xmmword ptr [rsp + 64], xmm4 # 16-byte Spill
	call	sqrt
	movapd	xmm4, xmmword ptr [rsp + 64] # 16-byte Reload
	movsd	xmm6, qword ptr [rsp + 40] # 8-byte Reload
                                        # xmm6 = mem[0],zero
	movapd	xmm1, xmmword ptr [rsp + 80] # 16-byte Reload
	xorpd	xmm7, xmm7
	movapd	xmm9, xmmword ptr [rsp + 48] # 16-byte Reload
	movsd	xmm8, qword ptr [rsp + 24] # 8-byte Reload
                                        # xmm8 = mem[0],zero
	movsd	xmm2, qword ptr [rsp + 32] # 8-byte Reload
                                        # xmm2 = mem[0],zero
.LBB36_11:                              #   in Loop: Header=BB36_8 Depth=1
	ucomisd	xmm4, xmm7
	jne	.LBB36_12
	jnp	.LBB36_15
.LBB36_12:                              #   in Loop: Header=BB36_8 Depth=1
	xorpd	xmm5, xmm5
	movsd	xmm3, qword ptr [rsp + 16] # 8-byte Reload
                                        # xmm3 = mem[0],zero
	ucomisd	xmm3, xmm4
	ja	.LBB36_14
# %bb.13:                               #   in Loop: Header=BB36_8 Depth=1
	movsd	xmm4, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm4 = mem[0],zero
	divsd	xmm4, xmm0
	movapd	xmm0, xmm6
	mulsd	xmm0, xmm4
	movapd	xmm3, xmm1
	mulsd	xmm3, xmm4
	movapd	xmm5, xmm1
	unpckhpd	xmm5, xmm1      # xmm5 = xmm5[1],xmm1[1]
	mulsd	xmm4, xmm5
	subsd	xmm6, xmm0
	subsd	xmm1, xmm3
	subsd	xmm5, xmm4
	mulsd	xmm6, xmm6
	mulsd	xmm1, xmm1
	addsd	xmm1, xmm6
	mulsd	xmm5, xmm5
	addsd	xmm5, xmm1
	mulsd	xmm5, qword ptr [rip + .LCPI36_3]
.LBB36_14:                              #   in Loop: Header=BB36_8 Depth=1
	movsd	xmm0, qword ptr [rsp]   # 8-byte Reload
                                        # xmm0 = mem[0],zero
	addsd	xmm0, xmm5
	movsd	qword ptr [rsp], xmm0   # 8-byte Spill
.LBB36_15:                              #   in Loop: Header=BB36_8 Depth=1
	add	rbx, 1
	add	r15, 24
	cmp	rbx, qword ptr [r14]
	jb	.LBB36_8
.LBB36_16:
	xorpd	xmm0, xmm0
	addsd	xmm2, xmm0
	addsd	xmm2, qword ptr [rsp]   # 8-byte Folded Reload
	movapd	xmm0, xmm2
	add	rsp, 96
	pop	rbx
	pop	r14
	pop	r15
	ret
.Lfunc_end36:
                                        # -- End function
non-virtual thunk to md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_force(md::system const&, md::array_view<md::vector>): # @non-virtual thunk to md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >::compute_force(md::system const&, md::array_view<md::vector>)
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 32
	mov	rbx, rdx
	mov	r14, rsi
	mov	r15, rdi
	lea	rdi, [rdi - 8]
	movups	xmm0, xmmword ptr [rdx]
	movaps	xmmword ptr [rsp + 16], xmm0
	lea	rdx, [rsp + 16]
	call	md::all_pair_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
	movups	xmm0, xmmword ptr [rbx]
	movaps	xmmword ptr [rsp], xmm0
	mov	rdx, rsp
	mov	rdi, r15
	mov	rsi, r14
	call	md::sphere_surface_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
	add	rsp, 32
	pop	rbx
	pop	r14
	pop	r15
	ret
.Lfunc_end37:
                                        # -- End function
.LCPI38_0:
.LCPI38_1:
.LCPI38_2:
md::all_pair_forcefield<my_forcefield>::compute_energy(md::system const&): # @md::all_pair_forcefield<my_forcefield>::compute_energy(md::system const&)
# %bb.0:
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	r8, qword ptr [rsi + 8]
	mov	rax, qword ptr [r8 + 8*rax]
	mov	rcx, qword ptr [rax + 8]
	mov	rax, qword ptr [rax + 16]
	sub	rax, rcx
	je	.LBB38_1
# %bb.2:
	sar	rax, 3
	movabs	r11, -6148914691236517205
	imul	r11, rax
	mov	r9, qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value]
	lea	r10, [rcx + 16]
	xorpd	xmm0, xmm0
	xor	eax, eax
	movsd	xmm8, qword ptr [rip + .LCPI38_0] # xmm8 = mem[0],zero
	movsd	xmm2, qword ptr [rip + .LCPI38_1] # xmm2 = mem[0],zero
	movsd	xmm9, qword ptr [rip + .LCPI38_2] # xmm9 = mem[0],zero
.LBB38_3:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB38_5 Depth 2
	test	rax, rax
	je	.LBB38_6
# %bb.4:                                #   in Loop: Header=BB38_3 Depth=1
	mov	rdx, qword ptr [r8 + 8*r9]
	mov	rsi, qword ptr [rdx + 8]
	movsd	xmm4, qword ptr [rsi + 8*rax] # xmm4 = mem[0],zero
	lea	rdx, [rax + 2*rax]
	movsd	xmm5, qword ptr [rcx + 8*rdx] # xmm5 = mem[0],zero
	movsd	xmm6, qword ptr [rcx + 8*rdx + 8] # xmm6 = mem[0],zero
	movhpd	xmm5, qword ptr [rcx + 8*rdx + 16] # xmm5 = xmm5[0],mem[0]
	mov	rdi, r10
	xor	edx, edx
.LBB38_5:                               #   Parent Loop BB38_3 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movsd	xmm7, qword ptr [rsi + 8*rdx] # xmm7 = mem[0],zero
	addsd	xmm7, xmm4
	movsd	xmm1, qword ptr [rdi - 16] # xmm1 = mem[0],zero
	movsd	xmm3, qword ptr [rdi - 8] # xmm3 = mem[0],zero
	subsd	xmm3, xmm6
	movhpd	xmm1, qword ptr [rdi]   # xmm1 = xmm1[0],mem[0]
	subpd	xmm1, xmm5
	mulsd	xmm3, xmm3
	mulpd	xmm1, xmm1
	addsd	xmm3, xmm1
	unpckhpd	xmm1, xmm1      # xmm1 = xmm1[1,1]
	addsd	xmm1, xmm3
	mulsd	xmm7, xmm7
	divsd	xmm1, xmm7
	movapd	xmm3, xmm1
	mulsd	xmm3, xmm1
	mulsd	xmm3, xmm1
	addsd	xmm3, xmm8
	movapd	xmm1, xmm2
	divsd	xmm1, xmm3
	addsd	xmm1, xmm9
	mulsd	xmm1, xmm1
	addsd	xmm0, xmm1
	add	rdx, 1
	add	rdi, 24
	cmp	rax, rdx
	jne	.LBB38_5
.LBB38_6:                               #   in Loop: Header=BB38_3 Depth=1
	add	rax, 1
	cmp	rax, r11
	jb	.LBB38_3
# %bb.7:
	ret
.LBB38_1:
	xorps	xmm0, xmm0
	ret
.Lfunc_end38:
                                        # -- End function
.LCPI39_0:
.LCPI39_1:
.LCPI39_2:
.LCPI39_3:
md::all_pair_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>): # @md::all_pair_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
# %bb.0:
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	r8, qword ptr [rsi + 8]
	mov	rax, qword ptr [r8 + 8*rax]
	mov	r15, qword ptr [rax + 8]
	mov	rax, qword ptr [rax + 16]
	sub	rax, r15
	je	.LBB39_6
# %bb.1:
	sar	rax, 3
	movabs	r10, -6148914691236517205
	imul	r10, rax
	mov	r9, qword ptr [rip + md::detail::type_hash::hash<radius_attribute_key>::value]
	xor	r13d, r13d
	movsd	xmm9, qword ptr [rip + .LCPI39_0] # xmm9 = mem[0],zero
	movsd	xmm8, qword ptr [rip + .LCPI39_1] # xmm8 = mem[0],zero
	movsd	xmm0, qword ptr [rip + .LCPI39_2] # xmm0 = mem[0],zero
	movsd	xmm3, qword ptr [rip + .LCPI39_3] # xmm3 = mem[0],zero
.LBB39_2:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB39_4 Depth 2
	test	r13, r13
	je	.LBB39_5
# %bb.3:                                #   in Loop: Header=BB39_2 Depth=1
	mov	rax, qword ptr [r8 + 8*r9]
	mov	r12, qword ptr [rax + 8]
	lea	rax, [8*r13]
	lea	rax, [rax + 2*rax]
	lea	r11, [r15 + rax]
	lea	r14, [r15 + rax + 16]
	mov	rbx, qword ptr [rdx]
	lea	rsi, [rbx + rax]
	lea	rcx, [rbx + rax]
	add	rcx, 16
	mov	edi, 2
	xor	eax, eax
.LBB39_4:                               #   Parent Loop BB39_2 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movsd	xmm7, qword ptr [r12 + 8*rax] # xmm7 = mem[0],zero
	addsd	xmm7, qword ptr [r12 + 8*r13]
	movupd	xmm1, xmmword ptr [r15 + 8*rdi - 16]
	movsd	xmm4, qword ptr [r15 + 8*rdi] # xmm4 = mem[0],zero
	movupd	xmm6, xmmword ptr [r11]
	subpd	xmm1, xmm6
	subsd	xmm4, qword ptr [r14]
	mulsd	xmm7, xmm7
	movapd	xmm6, xmm9
	divsd	xmm6, xmm7
	movapd	xmm7, xmm1
	mulsd	xmm7, xmm1
	movapd	xmm5, xmm1
	unpckhpd	xmm5, xmm1      # xmm5 = xmm5[1],xmm1[1]
	mulsd	xmm5, xmm5
	addsd	xmm5, xmm7
	movapd	xmm7, xmm4
	mulsd	xmm7, xmm4
	addsd	xmm7, xmm5
	mulsd	xmm7, xmm6
	movapd	xmm5, xmm7
	mulsd	xmm5, xmm7
	mulsd	xmm7, xmm5
	addsd	xmm7, xmm8
	movapd	xmm2, xmm0
	divsd	xmm2, xmm7
	mulsd	xmm6, xmm3
	mulsd	xmm6, xmm5
	movapd	xmm5, xmm2
	mulsd	xmm5, xmm2
	subsd	xmm5, xmm2
	mulsd	xmm5, xmm6
	mulsd	xmm4, xmm5
	unpcklpd	xmm5, xmm5      # xmm5 = xmm5[0,0]
	mulpd	xmm5, xmm1
	movupd	xmm1, xmmword ptr [rbx + 8*rdi - 16]
	addpd	xmm1, xmm5
	movupd	xmmword ptr [rbx + 8*rdi - 16], xmm1
	movsd	xmm1, qword ptr [rbx + 8*rdi] # xmm1 = mem[0],zero
	addsd	xmm1, xmm4
	movsd	qword ptr [rbx + 8*rdi], xmm1
	movupd	xmm1, xmmword ptr [rsi]
	subpd	xmm1, xmm5
	movupd	xmmword ptr [rsi], xmm1
	movsd	xmm1, qword ptr [rcx]   # xmm1 = mem[0],zero
	subsd	xmm1, xmm4
	movsd	qword ptr [rcx], xmm1
	add	rax, 1
	add	rdi, 3
	cmp	r13, rax
	jne	.LBB39_4
.LBB39_5:                               #   in Loop: Header=BB39_2 Depth=1
	add	r13, 1
	cmp	r13, r10
	jb	.LBB39_2
.LBB39_6:
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	ret
.Lfunc_end39:
                                        # -- End function
.LCPI40_0:
md::sphere_surface_forcefield<my_forcefield>::compute_energy(md::system const&): # @md::sphere_surface_forcefield<my_forcefield>::compute_energy(md::system const&)
# %bb.0:
	push	r15
	push	r14
	push	rbx
	sub	rsp, 96
	cmp	qword ptr [rsi], 0
	je	.LBB40_1
# %bb.3:
	mov	r14, rsi
	movsd	xmm8, qword ptr [rdi + 16] # xmm8 = mem[0],zero
	movupd	xmm5, xmmword ptr [rdi + 24]
	movsd	xmm0, qword ptr [rdi + 40] # xmm0 = mem[0],zero
	movsd	qword ptr [rsp + 16], xmm0 # 8-byte Spill
	mulsd	xmm0, xmm0
	movsd	qword ptr [rsp + 24], xmm0 # 8-byte Spill
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	rcx, qword ptr [rsi + 8]
	mov	rax, qword ptr [rcx + 8*rax]
	mov	rbx, qword ptr [rax + 8]
	add	rbx, 8
	xorps	xmm6, xmm6
	xor	r15d, r15d
	xorpd	xmm0, xmm0
	movsd	qword ptr [rsp + 8], xmm0 # 8-byte Spill
	movsd	qword ptr [rsp + 32], xmm8 # 8-byte Spill
	movapd	xmmword ptr [rsp + 48], xmm5 # 16-byte Spill
.LBB40_4:                               # =>This Inner Loop Header: Depth=1
	movsd	xmm4, qword ptr [rbx - 8] # xmm4 = mem[0],zero
	subsd	xmm4, xmm8
	movupd	xmm7, xmmword ptr [rbx]
	subpd	xmm7, xmm5
	movapd	xmm0, xmm4
	mulsd	xmm0, xmm4
	movapd	xmm3, xmm7
	mulpd	xmm3, xmm7
	addsd	xmm0, xmm3
	unpckhpd	xmm3, xmm3      # xmm3 = xmm3[1,1]
	addsd	xmm3, xmm0
	ucomisd	xmm3, xmm6
	jb	.LBB40_6
# %bb.5:                                #   in Loop: Header=BB40_4 Depth=1
	xorps	xmm0, xmm0
	sqrtsd	xmm0, xmm3
	jmp	.LBB40_7
.LBB40_6:                               #   in Loop: Header=BB40_4 Depth=1
	movapd	xmm0, xmm3
	movapd	xmmword ptr [rsp + 80], xmm7 # 16-byte Spill
	movsd	qword ptr [rsp + 40], xmm4 # 8-byte Spill
	movapd	xmmword ptr [rsp + 64], xmm3 # 16-byte Spill
	call	sqrt
	movapd	xmm3, xmmword ptr [rsp + 64] # 16-byte Reload
	movsd	xmm4, qword ptr [rsp + 40] # 8-byte Reload
                                        # xmm4 = mem[0],zero
	movapd	xmm7, xmmword ptr [rsp + 80] # 16-byte Reload
	xorps	xmm6, xmm6
	movapd	xmm5, xmmword ptr [rsp + 48] # 16-byte Reload
	movsd	xmm8, qword ptr [rsp + 32] # 8-byte Reload
                                        # xmm8 = mem[0],zero
.LBB40_7:                               #   in Loop: Header=BB40_4 Depth=1
	ucomisd	xmm3, xmm6
	jne	.LBB40_8
	jnp	.LBB40_11
.LBB40_8:                               #   in Loop: Header=BB40_4 Depth=1
	xorpd	xmm1, xmm1
	movsd	xmm2, qword ptr [rsp + 24] # 8-byte Reload
                                        # xmm2 = mem[0],zero
	ucomisd	xmm2, xmm3
	ja	.LBB40_10
# %bb.9:                                #   in Loop: Header=BB40_4 Depth=1
	movsd	xmm2, qword ptr [rsp + 16] # 8-byte Reload
                                        # xmm2 = mem[0],zero
	divsd	xmm2, xmm0
	movapd	xmm0, xmm4
	mulsd	xmm0, xmm2
	movapd	xmm3, xmm7
	mulsd	xmm3, xmm2
	movapd	xmm1, xmm7
	unpckhpd	xmm1, xmm7      # xmm1 = xmm1[1],xmm7[1]
	mulsd	xmm2, xmm1
	subsd	xmm4, xmm0
	subsd	xmm7, xmm3
	subsd	xmm1, xmm2
	mulsd	xmm4, xmm4
	mulsd	xmm7, xmm7
	addsd	xmm7, xmm4
	mulsd	xmm1, xmm1
	addsd	xmm1, xmm7
	mulsd	xmm1, qword ptr [rip + .LCPI40_0]
.LBB40_10:                              #   in Loop: Header=BB40_4 Depth=1
	movsd	xmm0, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm0 = mem[0],zero
	addsd	xmm0, xmm1
	movsd	qword ptr [rsp + 8], xmm0 # 8-byte Spill
.LBB40_11:                              #   in Loop: Header=BB40_4 Depth=1
	add	r15, 1
	add	rbx, 24
	cmp	r15, qword ptr [r14]
	jb	.LBB40_4
	jmp	.LBB40_2
.LBB40_1:
	xorpd	xmm0, xmm0
	movsd	qword ptr [rsp + 8], xmm0 # 8-byte Spill
.LBB40_2:
	movsd	xmm0, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm0 = mem[0],zero
	add	rsp, 96
	pop	rbx
	pop	r14
	pop	r15
	ret
.Lfunc_end40:
                                        # -- End function
.LCPI41_0:
.LCPI41_1:
md::sphere_surface_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>): # @md::sphere_surface_forcefield<my_forcefield>::compute_force(md::system const&, md::array_view<md::vector>)
# %bb.0:
	push	rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	sub	rsp, 88
	movups	xmm0, xmmword ptr [rdi + 16]
	movaps	xmmword ptr [rsp + 64], xmm0 # 16-byte Spill
	movsd	xmm0, qword ptr [rdi + 32] # xmm0 = mem[0],zero
	movsd	qword ptr [rsp + 40], xmm0 # 8-byte Spill
	movsd	xmm0, qword ptr [rdi + 40] # xmm0 = mem[0],zero
	movsd	qword ptr [rsp + 16], xmm0 # 8-byte Spill
	mov	rax, qword ptr [rip + md::detail::type_hash::hash<md::tag_position_attribute>::value]
	mov	rcx, qword ptr [rsi + 8]
	mov	rax, qword ptr [rcx + 8*rax]
	mov	r13, qword ptr [rax + 8]
	mov	qword ptr [rdi + 8], 0
	cmp	qword ptr [rsi], 0
	je	.LBB41_14
# %bb.1:
	mov	r14, rdx
	mov	r12, rsi
	mov	r15, rdi
	movsd	xmm0, qword ptr [rsp + 16] # 8-byte Reload
                                        # xmm0 = mem[0],zero
	mulsd	xmm0, xmm0
	movsd	qword ptr [rsp + 24], xmm0 # 8-byte Spill
	mov	ebx, 16
	xor	ebp, ebp
	xorps	xmm8, xmm8
.LBB41_2:                               # =>This Inner Loop Header: Depth=1
	movupd	xmm6, xmmword ptr [r13 + rbx - 16]
	subpd	xmm6, xmmword ptr [rsp + 64] # 16-byte Folded Reload
	movsd	xmm2, qword ptr [r13 + rbx] # xmm2 = mem[0],zero
	subsd	xmm2, qword ptr [rsp + 40] # 8-byte Folded Reload
	movapd	xmm0, xmm6
	mulpd	xmm0, xmm6
	movapd	xmm1, xmm0
	unpckhpd	xmm1, xmm0      # xmm1 = xmm1[1],xmm0[1]
	addsd	xmm1, xmm0
	movapd	xmm7, xmm2
	mulsd	xmm7, xmm2
	addsd	xmm7, xmm1
	ucomisd	xmm7, xmm8
	jb	.LBB41_4
# %bb.3:                                #   in Loop: Header=BB41_2 Depth=1
	xorps	xmm0, xmm0
	sqrtsd	xmm0, xmm7
	jmp	.LBB41_5
.LBB41_4:                               #   in Loop: Header=BB41_2 Depth=1
	movapd	xmm0, xmm7
	movsd	qword ptr [rsp + 8], xmm2 # 8-byte Spill
	movsd	qword ptr [rsp + 32], xmm7 # 8-byte Spill
	movapd	xmmword ptr [rsp + 48], xmm6 # 16-byte Spill
	call	sqrt
	movapd	xmm6, xmmword ptr [rsp + 48] # 16-byte Reload
	movsd	xmm7, qword ptr [rsp + 32] # 8-byte Reload
                                        # xmm7 = mem[0],zero
	movsd	xmm2, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm2 = mem[0],zero
	xorps	xmm8, xmm8
.LBB41_5:                               #   in Loop: Header=BB41_2 Depth=1
	ucomisd	xmm7, xmm8
	jne	.LBB41_6
	jnp	.LBB41_13
.LBB41_6:                               #   in Loop: Header=BB41_2 Depth=1
	movsd	xmm9, qword ptr [rsp + 16] # 8-byte Reload
                                        # xmm9 = mem[0],zero
	divsd	xmm9, xmm0
	movsd	xmm0, qword ptr [rsp + 24] # 8-byte Reload
                                        # xmm0 = mem[0],zero
	ucomisd	xmm0, xmm7
	jbe	.LBB41_8
# %bb.7:                                #   in Loop: Header=BB41_2 Depth=1
	xorpd	xmm0, xmm0
	xorpd	xmm10, xmm10
	jmp	.LBB41_9
.LBB41_8:                               #   in Loop: Header=BB41_2 Depth=1
	movapd	xmm3, xmm2
	mulsd	xmm3, xmm9
	movapd	xmm10, xmm2
	subsd	xmm10, xmm3
	mulsd	xmm10, qword ptr [rip + .LCPI41_1]
	movapd	xmm3, xmm9
	unpcklpd	xmm3, xmm9      # xmm3 = xmm3[0],xmm9[0]
	mulpd	xmm3, xmm6
	movapd	xmm0, xmm6
	subpd	xmm0, xmm3
	mulpd	xmm0, xmmword ptr [rip + .LCPI41_0]
.LBB41_9:                               #   in Loop: Header=BB41_2 Depth=1
	movapd	xmm3, xmm6
	mulsd	xmm3, xmm0
	movapd	xmm4, xmm0
	unpckhpd	xmm4, xmm0      # xmm4 = xmm4[1],xmm0[1]
	movapd	xmm5, xmm6
	unpckhpd	xmm5, xmm6      # xmm5 = xmm5[1],xmm6[1]
	mulsd	xmm5, xmm4
	addsd	xmm5, xmm3
	movapd	xmm1, xmm2
	mulsd	xmm1, xmm10
	addsd	xmm1, xmm5
	movapd	xmm3, xmm1
	divsd	xmm3, xmm7
	mulsd	xmm2, xmm3
	unpcklpd	xmm3, xmm3      # xmm3 = xmm3[0,0]
	mulpd	xmm3, xmm6
	subpd	xmm3, xmm0
	subsd	xmm2, xmm10
	movapd	xmm4, xmm9
	unpcklpd	xmm4, xmm9      # xmm4 = xmm4[0],xmm9[0]
	mulpd	xmm4, xmm3
	mulsd	xmm2, xmm9
	addpd	xmm4, xmm0
	addsd	xmm2, xmm10
	mov	rax, qword ptr [r14]
	movupd	xmm0, xmmword ptr [rax + rbx - 16]
	addpd	xmm0, xmm4
	movupd	xmmword ptr [rax + rbx - 16], xmm0
	addsd	xmm2, qword ptr [rax + rbx]
	movsd	qword ptr [rax + rbx], xmm2
	ucomisd	xmm7, xmm8
	jb	.LBB41_11
# %bb.10:                               #   in Loop: Header=BB41_2 Depth=1
	xorps	xmm0, xmm0
	sqrtsd	xmm0, xmm7
	jmp	.LBB41_12
.LBB41_11:                              #   in Loop: Header=BB41_2 Depth=1
	movapd	xmm0, xmm7
	movsd	qword ptr [rsp + 8], xmm1 # 8-byte Spill
	call	sqrt
	movsd	xmm1, qword ptr [rsp + 8] # 8-byte Reload
                                        # xmm1 = mem[0],zero
	xorps	xmm8, xmm8
.LBB41_12:                              #   in Loop: Header=BB41_2 Depth=1
	divsd	xmm1, xmm0
	movsd	xmm0, qword ptr [r15 + 8] # xmm0 = mem[0],zero
	subsd	xmm0, xmm1
	movsd	qword ptr [r15 + 8], xmm0
.LBB41_13:                              #   in Loop: Header=BB41_2 Depth=1
	add	rbp, 1
	add	rbx, 24
	cmp	rbp, qword ptr [r12]
	jb	.LBB41_2
.LBB41_14:
	add	rsp, 88
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
.Lfunc_end41:
                                        # -- End function
my_forcefield::~my_forcefield():                 # @my_forcefield::~my_forcefield()
# %bb.0:
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end42:
                                        # -- End function
non-virtual thunk to my_forcefield::~my_forcefield():            # @non-virtual thunk to my_forcefield::~my_forcefield()
# %bb.0:
	ret
.Lfunc_end43:
                                        # -- End function
non-virtual thunk to my_forcefield::~my_forcefield():            # @non-virtual thunk to my_forcefield::~my_forcefield()
# %bb.0:
	add	rdi, -8
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end44:
                                        # -- End function
md::system::add_forcefield(std::__1::shared_ptr<md::forcefield>): # @md::system::add_forcefield(std::__1::shared_ptr<md::forcefield>)
.Lfunc_begin8:
# %bb.0:
	push	r14
	push	rbx
	sub	rsp, 24
	mov	rax, qword ptr [rsi]
	mov	qword ptr [rsp + 8], rax
	mov	rcx, qword ptr [rsi + 8]
	mov	qword ptr [rsp + 16], rcx
	test	rcx, rcx
	je	.LBB45_2
# %bb.1:
	lock		add	qword ptr [rcx + 8], 1
.LBB45_2:
	test	rax, rax
	je	.LBB45_16
# %bb.3:
	mov	rcx, qword ptr [rdi + 48]
	cmp	rcx, qword ptr [rdi + 56]
	je	.LBB45_7
# %bb.4:
	mov	qword ptr [rcx], rax
	mov	rax, qword ptr [rsp + 16]
	mov	qword ptr [rcx + 8], rax
	test	rax, rax
	je	.LBB45_6
# %bb.5:
	lock		add	qword ptr [rax + 8], 1
	mov	rcx, qword ptr [rdi + 48]
.LBB45_6:
	add	rcx, 16
	mov	qword ptr [rdi + 48], rcx
	jmp	.LBB45_8
.LBB45_7:
	add	rdi, 40
.Ltmp46:
	lea	rsi, [rsp + 8]
	call	void std::__1::vector<std::__1::shared_ptr<md::forcefield>, std::__1::allocator<std::__1::shared_ptr<md::forcefield> > >::__push_back_slow_path<std::__1::shared_ptr<md::forcefield> const&>(std::__1::shared_ptr<md::forcefield> const&)
.Ltmp47:
.LBB45_8:
	mov	rbx, qword ptr [rsp + 16]
	test	rbx, rbx
	je	.LBB45_11
# %bb.9:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB45_11
# %bb.10:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB45_11:
	add	rsp, 24
	pop	rbx
	pop	r14
	ret
.LBB45_16:
	mov	edi, offset .L.str.5
	mov	esi, offset .L.str.6
	mov	ecx, offset .L__PRETTY_FUNCTION__._ZN2md6detail14sum_forcefield3addENSt3__110shared_ptrINS_10forcefieldEEE
	mov	edx, 1882
	call	__assert_fail
.LBB45_12:
.Ltmp48:
	mov	r14, rax
	mov	rbx, qword ptr [rsp + 16]
	test	rbx, rbx
	je	.LBB45_15
# %bb.13:
	mov	rax, -1
	lock		xadd	qword ptr [rbx + 8], rax
	test	rax, rax
	jne	.LBB45_15
# %bb.14:
	mov	rax, qword ptr [rbx]
	mov	rdi, rbx
	call	qword ptr [rax + 16]
	mov	rdi, rbx
	call	std::__1::__shared_weak_count::__release_weak()
.LBB45_15:
	mov	rdi, r14
	call	_Unwind_Resume
.Lfunc_end45:
GCC_except_table45:
.Lexception8:
.Lcst_begin8:
.Lcst_end8:
                                        # -- End function
std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::~__shared_ptr_emplace(): # @std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::~__shared_ptr_emplace()
# %bb.0:
	jmp	std::__1::__shared_weak_count::~__shared_weak_count() # TAILCALL
.Lfunc_end46:
                                        # -- End function
std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::~__shared_ptr_emplace(): # @std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::~__shared_ptr_emplace()
# %bb.0:
	push	rbx
	mov	rbx, rdi
	call	std::__1::__shared_weak_count::~__shared_weak_count()
	mov	rdi, rbx
	pop	rbx
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end47:
                                        # -- End function
std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::__on_zero_shared(): # @std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::__on_zero_shared()
# %bb.0:
	mov	rax, qword ptr [rdi + 24]
	add	rdi, 24
	jmp	qword ptr [rax]         # TAILCALL
.Lfunc_end48:
                                        # -- End function
std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::__on_zero_shared_weak(): # @std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >::__on_zero_shared_weak()
# %bb.0:
	jmp	operator delete(void*)                  # TAILCALL
.Lfunc_end49:
                                        # -- End function
void std::__1::vector<std::__1::shared_ptr<md::forcefield>, std::__1::allocator<std::__1::shared_ptr<md::forcefield> > >::__push_back_slow_path<std::__1::shared_ptr<md::forcefield> const&>(std::__1::shared_ptr<md::forcefield> const&): # @void std::__1::vector<std::__1::shared_ptr<md::forcefield>, std::__1::allocator<std::__1::shared_ptr<md::forcefield> > >::__push_back_slow_path<std::__1::shared_ptr<md::forcefield> const&>(std::__1::shared_ptr<md::forcefield> const&)
# %bb.0:
	push	rbp
	push	r15
	push	r14
	push	r13
	push	r12
	push	rbx
	push	rax
	mov	r14, rdi
	mov	r12, qword ptr [rdi]
	mov	rbx, qword ptr [rdi + 8]
	mov	rbp, rbx
	sub	rbp, r12
	sar	rbp, 4
	lea	rax, [rbp + 1]
	mov	rcx, rax
	shr	rcx, 60
	jne	.LBB50_24
# %bb.1:
	mov	r15, rsi
	movabs	rcx, 1152921504606846975
	mov	r13, qword ptr [r14 + 16]
	sub	r13, r12
	mov	rdx, r13
	sar	rdx, 4
	movabs	rsi, 576460752303423486
	cmp	rdx, rsi
	ja	.LBB50_2
# %bb.3:
	sar	r13, 3
	cmp	r13, rax
	cmovb	r13, rax
	test	r13, r13
	je	.LBB50_4
# %bb.5:
	cmp	r13, rcx
	jbe	.LBB50_6
# %bb.25:
	mov	edi, offset .L.str
	call	std::__1::__throw_length_error(char const*)
.LBB50_2:
	mov	r13, rcx
.LBB50_6:
	mov	rdi, r13
	shl	rdi, 4
	call	operator new(unsigned long)
.LBB50_7:
	shl	rbp, 4
	lea	rcx, [rax + rbp]
	shl	r13, 4
	mov	rdx, qword ptr [r15]
	mov	qword ptr [rax + rbp], rdx
	mov	rdx, qword ptr [r15 + 8]
	mov	qword ptr [rax + rbp + 8], rdx
	test	rdx, rdx
	je	.LBB50_9
# %bb.8:
	lock		add	qword ptr [rdx + 8], 1
	mov	r12, qword ptr [r14]
	mov	rbx, qword ptr [r14 + 8]
.LBB50_9:
	add	rax, r13
	lea	rdx, [rcx + 16]
	cmp	rbx, r12
	je	.LBB50_17
# %bb.10:
	lea	rsi, [rbx - 16]
	sub	rsi, r12
	mov	edi, esi
	shr	edi, 4
	add	edi, 1
	and	rdi, 7
	je	.LBB50_13
# %bb.11:
	neg	rdi
	xorps	xmm0, xmm0
.LBB50_12:                              # =>This Inner Loop Header: Depth=1
	movups	xmm1, xmmword ptr [rbx - 16]
	movups	xmmword ptr [rcx - 16], xmm1
	add	rcx, -16
	movups	xmmword ptr [rbx - 16], xmm0
	lea	rbx, [rbx - 16]
	inc	rdi
	jne	.LBB50_12
.LBB50_13:
	cmp	rsi, 112
	jb	.LBB50_16
# %bb.14:
	xorps	xmm0, xmm0
.LBB50_15:                              # =>This Inner Loop Header: Depth=1
	movups	xmm1, xmmword ptr [rbx - 16]
	movups	xmmword ptr [rcx - 16], xmm1
	movups	xmmword ptr [rbx - 16], xmm0
	movups	xmm1, xmmword ptr [rbx - 32]
	movups	xmmword ptr [rcx - 32], xmm1
	movups	xmmword ptr [rbx - 32], xmm0
	movups	xmm1, xmmword ptr [rbx - 48]
	movups	xmmword ptr [rcx - 48], xmm1
	movups	xmmword ptr [rbx - 48], xmm0
	movups	xmm1, xmmword ptr [rbx - 64]
	movups	xmmword ptr [rcx - 64], xmm1
	movups	xmmword ptr [rbx - 64], xmm0
	movups	xmm1, xmmword ptr [rbx - 80]
	movups	xmmword ptr [rcx - 80], xmm1
	movups	xmmword ptr [rbx - 80], xmm0
	movups	xmm1, xmmword ptr [rbx - 96]
	movups	xmmword ptr [rcx - 96], xmm1
	movups	xmmword ptr [rbx - 96], xmm0
	movups	xmm1, xmmword ptr [rbx - 112]
	movups	xmmword ptr [rcx - 112], xmm1
	movups	xmmword ptr [rbx - 112], xmm0
	movups	xmm1, xmmword ptr [rbx - 128]
	movups	xmmword ptr [rcx - 128], xmm1
	add	rcx, -128
	movups	xmmword ptr [rbx - 128], xmm0
	lea	rbx, [rbx - 128]
	cmp	r12, rbx
	jne	.LBB50_15
.LBB50_16:
	mov	r12, qword ptr [r14]
	mov	rbx, qword ptr [r14 + 8]
.LBB50_17:
	mov	qword ptr [r14], rcx
	mov	qword ptr [r14 + 8], rdx
	mov	qword ptr [r14 + 16], rax
	cmp	rbx, r12
	je	.LBB50_22
.LBB50_18:                              # =>This Inner Loop Header: Depth=1
	mov	r14, qword ptr [rbx - 8]
	add	rbx, -16
	test	r14, r14
	je	.LBB50_21
# %bb.19:                               #   in Loop: Header=BB50_18 Depth=1
	mov	rax, -1
	lock		xadd	qword ptr [r14 + 8], rax
	test	rax, rax
	je	.LBB50_20
.LBB50_21:                              #   in Loop: Header=BB50_18 Depth=1
	cmp	r12, rbx
	jne	.LBB50_18
	jmp	.LBB50_22
.LBB50_20:                              #   in Loop: Header=BB50_18 Depth=1
	mov	rax, qword ptr [r14]
	mov	rdi, r14
	call	qword ptr [rax + 16]
	mov	rdi, r14
	call	std::__1::__shared_weak_count::__release_weak()
	cmp	r12, rbx
	jne	.LBB50_18
.LBB50_22:
	test	r12, r12
	je	.LBB50_23
# %bb.26:
	mov	rdi, r12
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	jmp	operator delete(void*)                  # TAILCALL
.LBB50_23:
	add	rsp, 8
	pop	rbx
	pop	r12
	pop	r13
	pop	r14
	pop	r15
	pop	rbp
	ret
.LBB50_4:
	xor	r13d, r13d
	xor	eax, eax
	jmp	.LBB50_7
.LBB50_24:
	mov	rdi, r14
	call	std::__1::__vector_base_common<true>::__throw_length_error() const
.Lfunc_end50:
                                        # -- End function
radius_attribute:

md::detail::type_hash::hash<md::tag_mass_attribute>::value:

guard variable for md::detail::type_hash::hash<md::tag_mass_attribute>::value:

md::detail::type_hash::hash<md::tag_mobility_attribute>::value:

guard variable for md::detail::type_hash::hash<md::tag_mobility_attribute>::value:

md::detail::type_hash::hash<md::tag_position_attribute>::value:

guard variable for md::detail::type_hash::hash<md::tag_position_attribute>::value:

md::detail::type_hash::hash<md::tag_velocity_attribute>::value:

guard variable for md::detail::type_hash::hash<md::tag_velocity_attribute>::value:

md::detail::type_hash::hash<radius_attribute_key>::value:

guard variable for md::detail::type_hash::hash<radius_attribute_key>::value:

.L.str:

vtable for md::detail::sum_forcefield:

typeinfo name for md::detail::sum_forcefield:

typeinfo name for md::forcefield:

typeinfo for md::forcefield:

typeinfo for md::detail::sum_forcefield:

vtable for md::detail::array_erasure::instance<double>:

typeinfo name for md::detail::array_erasure::instance<double>:

typeinfo name for md::detail::array_erasure:

typeinfo for md::detail::array_erasure:

typeinfo for md::detail::array_erasure::instance<double>:

vtable for md::detail::array_erasure::instance<md::point>:

typeinfo name for md::detail::array_erasure::instance<md::point>:

typeinfo for md::detail::array_erasure::instance<md::point>:

vtable for md::detail::array_erasure::instance<md::vector>:

typeinfo name for md::detail::array_erasure::instance<md::vector>:

typeinfo for md::detail::array_erasure::instance<md::vector>:

vtable for my_forcefield:

typeinfo name for md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >:

typeinfo name for md::all_pair_forcefield<my_forcefield>:

typeinfo for md::all_pair_forcefield<my_forcefield>:

typeinfo name for md::sphere_surface_forcefield<my_forcefield>:

typeinfo for md::sphere_surface_forcefield<my_forcefield>:

typeinfo for md::composite_forcefield<md::all_pair_forcefield<my_forcefield>, md::sphere_surface_forcefield<my_forcefield> >:

typeinfo name for my_forcefield:

typeinfo for my_forcefield:

md::detail::type_hash::get_counter()::counter:

vtable for std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >:

typeinfo name for std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >:

typeinfo for std::__1::__shared_ptr_emplace<my_forcefield, std::__1::allocator<my_forcefield> >:

.L.str.5:

.L.str.6:

.L__PRETTY_FUNCTION__._ZN2md6detail14sum_forcefield3addENSt3__110shared_ptrINS_10forcefieldEEE:


