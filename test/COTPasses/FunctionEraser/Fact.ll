
; RUN: opt -load %projshlibdir/COTPasses.so     \
; RUN:     -function-eraser -erase-threshold=10 \
; RUN:     -S -o - %s | FileCheck %s
; REQUIRES: loadable_module

target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

define i32 @fact(i32 %n) nounwind {
  %1 = alloca i32, align 4
  store i32 %n, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = icmp eq i32 %2, 0
  br i1 %3, label %4, label %5

; <label>:4                                       ; preds = %0
  br label %11

; <label>:5                                       ; preds = %0
  %6 = load i32* %1, align 4
  %7 = load i32* %1, align 4
  %8 = sub i32 %7, 1
  %9 = call i32 @fact(i32 %8)
  %10 = mul i32 %6, %9
  br label %11

; <label>:11                                      ; preds = %5, %4
  %12 = phi i32 [ 1, %4 ], [ %10, %5 ]
  ret i32 %12
}

;CHECK: declare i32 @fact(i32) nounwind
