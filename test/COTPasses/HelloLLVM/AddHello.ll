
; RUN: opt -load %projshlibdir/COTPasses.so \
; RUN:     -hello-llvm                      \
; RUN:     -S -o - %s | FileCheck %s
; REQUIRES: loadable_module

target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

define void @foo() nounwind {
  ret void
}

; CHECK: @hello_world.msg = internal constant [15 x i8] c"Hello, World!\0A\00"

; CHECK:      define void @hello_world() {
; CHECK-NEXT: entry:
; CHECK-NEXT:   %0 = getelementptr [15 x i8]* @hello_world.msg, i32 0, i32 0
; CHECK-NEXT:   %1 = call i32 (i8*, ...)* @printf(i8* %0)
; CHECK-NEXT:   ret void
; CHECK-NEXT: }

; CHECK: declare i32 @printf(i8*, ...)

; CHECK:      define void @main() {
; CHECK-NEXT: entry:
; CHECK-NEXT:   call void @hello_world()
; CHECK-NEXT:   ret void
; CHECK-NEXT: }
