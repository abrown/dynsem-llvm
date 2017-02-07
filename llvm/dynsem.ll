; adapted from https://github.com/johncosker/llvm-binarysearch/blob/master/bins.llvm



; external functions
declare i32 @printf(i8*,...); export the prototype of printf
declare i64* @malloc(i64); export the prototype of malloc



; types 
@enum.symbol = constant i8 0
%types.symbol = type {i32, i8*}; length, value

@enum.constant = constant i8 1
%types.constant = type {i32, i8*}; length, value

@enum.variable = constant i8 2
%types.map_entry = type {%types.ast*, %types.ast*}; from, to
%types.variable = type {%types.symbol*, %types.map_entry}; name, from_to_mapping

@enum.wildcard = constant i8 3
%types.wildcard = type {}

@enum.constructor = constant i8 4
%types.constructor = type {%types.symbol*, i32, [0 x %types.constructor*]}; name, length, children

%types.ast = type {i8, %types.constant*}; type, value (will bitcast from constant to ...)

@enum.tuple = constant i8 5
%types.tuple = type {i32, [0 x %types.ast*]}; length, children



; functions
define %types.ast* @match(%types.ast* %source){
entry:
  ; decision tree here

  ret %types.ast* null
}

define %types.ast* @clone(%types.ast* %source) {
  entry:
    %new = call i64* @malloc(i64 9)
    %dest = bitcast i64* %new to %types.ast*

    %source.type = getelementptr %types.ast* %source, i64 0, i32 0
    %type = load i8* %source.type
    switch i8 %type, label %error [
      i8 0, label %symbol_constant_wildcard
      i8 1, label %symbol_constant_wildcard
      i8 3, label %symbol_constant_wildcard
      i8 4, label %constructor
      i8 2, label %variable
      i8 5, label %tuple
    ]

    symbol_constant_wildcard:
      ; do nothing, assume these are immutable and kept in some constant pool
      %source.i8 = bitcast %types.ast* %source to i8*
      %dest.i8 = bitcast %types.ast* %dest to i8*
      call void %llvm.memcpy.p0i8.p0i8.i32(i8* %dest.i8, i8* %source.i8, i32 9, i32 0, i1 0);
      br label %return

    variable:
      br label %return

    constructor:
      br label %return

    tuple:
      br label %return

    error:
      ; some log here
      ret %types.ast* null

    return:
      ret %types.ast* %dest
}

define %types.ast* @replace(%types.ast* %source.rule, %types.ast* %source.runtime, %types.ast* %dest.runtime) {
entry:
  ; use entry map to replace variables in cloned destination with runtime source
  ret %types.ast* null
}

define void @clean() {
entry:
  ; remove unused pointers from memory; counters?
  ret void
}

define i32 @binSearch(i32* %start, i32 %n, i32 %k){
entry:
  ;Get the mid point of the array
    %mid = udiv i32 %n, 2
    ;Get mid value address
    %ptr = getelementptr i32* %start,i32 %mid
    ;Get mid value from address
    %midv = load i32* %ptr
    br label %checkeq

    checkeq:
    ;Check if %mid = %k, if so, %return
    %equal = icmp eq i32 %midv, %k
    br i1 %equal, label %return, label %checkn

    checkn:
    ;Check if %n = 1, if so that means there is nowhere left to search %return -1
    %nis1 = icmp eq i32 %n, 1
    br i1 %nis1, label %return, label %checkgtlt

    checkgtlt:
    ;If %k > %midv, go to gt branch, if it's < then go to lt branch
    %gtlt = icmp ugt i32 %k, %midv
    br i1 %gtlt, label %gt, label %lt

    gt:
    ;New starting position will be %mid + 1
    %t0 = add i32 %mid, 1
    ;Get ptr to new start
    %gnewstart = getelementptr i32* %start, i32 %t0
    ;New number of elements will be %mid -1
    %t1 = sub i32 %mid, 1
    ;next line is probably not needed
    %gnewn = sub i32 %n, %t1
    ;Recursively call search on the new array
    %gcall = call i32 @binSearch(i32* %gnewstart, i32 %gnewn, i32 %k) 
    br label %gtcheck

    ;The next few labels could probably be condensed, but it's a little easier to handle this way
    gtcheck:
    ;If the recursive call %returns -1, go to gtno, else go to gtyes
    %gcheck = icmp eq i32 %gcall, -1
    br i1 %gcheck, label %gtno, label %gtyes

    gtno:
    ;To make the phi easier
    br label %return

    gtyes:
    ;Not sure if this is needed or not, it's either add 1 or not, need to test
    ;because of shifting the mid point to make the new array
    %t2 = add i32 %mid, 1
    ;Add %gcall to %mid (+1 maybe) to preserve the position
    %gtret = add i32 %gcall, %t2
    br label %return

    lt:
    ;New %n is %n-1
    %lnewn = sub i32 %mid, 1
    ;Call recursively on the new array
    %lcall = call i32 @binSearch(i32* %start, i32 %lnewn, i32 %k)  
    br label %ltcheck

    ltcheck:
    ;If you get a -1 go to ltno else goto ltyes
    %lcheck = icmp eq i32 %lcall, -1
    br i1 %lcheck, label %ltno, label %ltyes

    ;These probably aren't needed
    ltno:
    br label %return

    ltyes:
    br label %return


    return:
    %retv = phi i32 [%mid, %checkeq], [-1, %checkn], [%gcall, %gtno], [%gtret, %gtyes], [%lcall, %ltyes], [%lcall, %ltno]
    ret i32 %retv
}

define i32 @main() {
  ;%t0 = bitcast [37 x i32]* @data to i32*
  ;%t1 = call i32 @binSearch(i32* %t0,i32 37,i32 49)
  ;%t2 = call i32(i8*,...)* @printf(i8* bitcast ([9 x i8]* @msg to i8*),i32 %t1)
  ;%t1 = call i32 @match()
  ret i32 42
}
