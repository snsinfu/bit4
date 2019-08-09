main:
	push	r12
	mov	rsi, QWORD PTR my::type_hash::counter()::counter_[rip]
	mov	edi, OFFSET FLAT:.LC0
	mov	r12, QWORD PTR my::type_hash::hash_v<main::foo_tag>[rip]
	push	rbp
	xor	eax, eax
	push	rbx
	mov	rbp, QWORD PTR my::type_hash::hash_v<main::bar_tag>[rip]
	mov	rbx, QWORD PTR my::type_hash::hash_v<main::baz_tag>[rip]
	call	printf
	mov	rsi, r12
	mov	edi, OFFSET FLAT:.LC1
	xor	eax, eax
	call	printf
	mov	rsi, rbp
	mov	edi, OFFSET FLAT:.LC2
	xor	eax, eax
	call	printf
	mov	rsi, rbx
	mov	edi, OFFSET FLAT:.LC3
	xor	eax, eax
	call	printf
	pop	rbx
	xor	eax, eax
	pop	rbp
	pop	r12
	ret
_GLOBAL__sub_I_main:
	mov	rax, QWORD PTR my::type_hash::counter()::counter_[rip]
	lea	rdx, [rax+1]
	mov	QWORD PTR my::type_hash::hash_v<main::foo_tag>[rip], rax
	mov	QWORD PTR my::type_hash::hash_v<main::bar_tag>[rip], rdx
	lea	rdx, [rax+3]
	add	rax, 2
	mov	QWORD PTR my::type_hash::hash_v<main::baz_tag>[rip], rax
	mov	QWORD PTR my::type_hash::counter()::counter_[rip], rdx
	ret
my::type_hash::counter()::counter_:
