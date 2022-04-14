# Computer architecture projects

## Index
- project1-Simple Calculator

## project1-Simple Calculator

### Description

본 프로젝트는 폰 노이만 구조와 ISA(Instruction Set Architecture)을 활용하여 간단한 사칙 연산과 MIPS CPU안에 있는 instruction중 Move, Compare, Jump, Branch 등을 직접 구현하고 출력하여 변수(레지스터)의 상태 변화를 볼 수 있는 프로그램이다. 또한 gcd.txt파일에 앞서 선언한 instruction set을 이용하여 두 수의 최대공약수를 구할수 있도록 하는 프로그램을 작성했다.

### Requirements for programming

- Instruction set은 .txt파일 안에 들어있고 opcode operand1 operand2의 구조를 띈다(e.g : + 0x2 0x4 , M R0 0x1).
- 각각의 Instruction들은 개행으로 구분된다.
- instruction set이 .txt파일 안에 들어가 있기 때문에 파일을 읽어오는 fopen, fgets와 같은 함수를 사용한다.
- operand에 16진수 형태의 문자열로 들어오기 때문에, 이를 long type으로 바꿔주는 strtol함수를 사용한다.

### Instructions

- Move(M) : 해당 레지스터(변수)에 값을 대입하는 instruction이다. (e.g : M R0 0x1 -> R0 = 1)
- Compare(C) : operand1 과 operand2의 값을 비교하여 operand1의 값이 크거나 같다면 R0레지스터에 1을, 아니라면 0을 대입한다. (e.g : C 0xa 0xc -> R0 = 0)
- Jump(J) : operand1에 쓰여있는 주소로 넘어간다. (e.g : J 0x2 -> .txt파일의 2번째 줄로 이동)
- Branch(B) : R0레지스터의 상태가 1이라면 operand1에 쓰여있는 주소로 Jump한다. (e.g : M R0 0x1, B 0x7 -> .txt파일의 7번째 줄로 이동)
- gcd : 최대공약수를 계산하기 위한 준비를 한다. operand1과 operand2에 있는 값을 R1, R2레지스터에 저장한다. (e.g : gcd 0x9 0x3 -> R1 = 9, R2 = 3)
- Same(S) : GCD계산을 위해 만든 instruction으로 operand1, operand2의 값이 같다면 R0레지스터에 1을, 같지 않다면 0을 대입한다. (e.g : S 0x4 0x4 -> R0 = 1)
- Halt(H) : 프로그램을 종료한다. (e.g : H -> 프로그램 종료)

### Enviroment

- 개발환경 : windows
- 사용 언어 : C

## project2. Single_Cycle_MIPS
