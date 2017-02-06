# VR88M

vr88m is a quick&dirty disassembler for V8 microRISC architecture.

## Howto

```
$ make
$ bin/vr88m -i my_binary_file.bin [-b ab12]
```

* -i : input file
* -b : base address

## Example

```
$ vr88m -i my_binary.bin | expand -t 10
0000:     bf 35 81  JSR       0x8135
0003:     ba        RTI
0004:     85        PSH       R5
0005:     ea 7b 80  LDA       R2, 0x807b
0007:     e1 13     LDI       R1, 0x13
```

```
$ vr88m -i my_binary.bin -b 1234 | expand -t 10
1234:     bf 35 81  JSR       0x8135
1237:     ba        RTI
1238:     85        PSH       R5
1239:     ea 7b 80  LDA       R2, 0x807b
123b:     e1 13     LDI       R1, 0x13
```
