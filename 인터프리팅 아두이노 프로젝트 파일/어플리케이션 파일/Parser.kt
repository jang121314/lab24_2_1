package com.example.arduinterpreterapplication

class Parser {
    companion object {
        const val INPUT_TEXT_MAX = 8096
        const val OUTPUT_TEXT_MAX = 512
        const val MAX_32 = 32
        const val MAX_4 = 4
        private var firstCase = true

        // 키워드 매핑
        private val keywords = Array(MAX_32) { index ->
            when (index) {
                1 -> "delay"
                2 -> "sum"
                3 -> "com"
                4 -> "point"
                5 -> "jump"
                // 6 -> "int"
                7 -> "fun"
                9 -> "exit"
                17 -> "pinMode"
                18 -> "digitalRead"
                19 -> "digitalWrite"
                20 -> "analogRead"
                21 -> "analogWrite"
                22 -> "tone"
                23 -> "noTone"
                else -> ""
            }
        }

        // 특수 키워드 매핑
        private val specialKeys = arrayOf(
            "main:", "setup:", "loop:", "end:"
        )
    }

    var inputText = CharArray(INPUT_TEXT_MAX)

    fun setInput(text: String) {
        if (text.length >= INPUT_TEXT_MAX) {
            throw Exception("Input text too long")
        }
        inputText = CharArray(INPUT_TEXT_MAX) // 새로운 배열 생성
        text.toCharArray().copyInto(inputText) // 복사
    }

    private var outputChar = CharArray(OUTPUT_TEXT_MAX)
    private var top = 0
    private var vindex = 0

    private fun newlines() {
        for (i in inputText.indices) {
            if (inputText[i] == '\n' || inputText[i] == '\r') {
                inputText[i] = ' '
            }
        }
    }

    private fun inputSpecialKey(i: Int) {
        when (i) {
            0, 1 -> { // main, setup
                outputChar[top++] = 7.toChar()
                outputChar[top++] = 97.toChar()
            }
            2 -> { // loop
                outputChar[top++] = 7.toChar()
                outputChar[top++] = 34.toChar()
                outputChar[top++] = 7.toChar()
                outputChar[top++] = 98.toChar()
            }
            3 -> { // end
                outputChar[top++] = 7.toChar()
                outputChar[top++] = 34.toChar()
            }
        }
    }

    private fun specialKeyCompare(text: String, max: Int) {
        if (max < 4) return
        for (i in 0 until MAX_4) {
            if (specialKeys[i].length < max &&
                text.startsWith(specialKeys[i])) {
                inputSpecialKey(i)
                vindex += specialKeys[i].length - 1
                break
            }
        }
    }

    private fun wordCompare(char: Char, text: String, max: Int) {
        for (i in 0 until MAX_32) {
            if (keywords[i].isEmpty()) continue
            if (char == keywords[i][0] &&
                keywords[i].length < max &&
                text.startsWith(keywords[i])) {
                outputChar[top++] = i.toChar()
                vindex += keywords[i].length - 1
                break
            }
        }
    }

    private fun processCase(text: String) {

        if (firstCase) {
            outputChar[top++] = 7.toChar()
            outputChar[top++] = 33.toChar()
            firstCase = false
        }

        val parts = text.trim().split(" ")
        if (parts.size == 2 && parts[0] == "case") {
            val numStr = parts[1].replace(":", "")
            val num = numStr.toIntOrNull()
            if (num != null) {
                outputChar[top++] = 7.toChar()
                outputChar[top++] = (num + 64).toChar()
            }
        }
    }

    private fun processIntCommand() {
        // 공백을 건너뛰어 첫 번째 숫자 찾기
        while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

        // 첫 번째 숫자 읽기
        var num1 = 0
        while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
            num1 = num1 * 10 + (inputText[vindex] - '0')
            vindex++
        }
        outputChar[top++] = num1.toChar()

        // 공백을 건너뛰어 두 번째 숫자 찾기
        while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

        // 두 번째 숫자 읽기
        var num2 = 0
        while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
            num2 = num2 * 10 + (inputText[vindex] - '0')
            vindex++
        }

        // 두 번째 숫자를 4개의 char로 분리
        outputChar[top++] = (num2 / 16777216 % 256).toChar()
        outputChar[top++] = (num2 / 65536 % 256).toChar()
        outputChar[top++] = (num2 / 256 % 256).toChar()
        outputChar[top++] = (num2 % 256).toChar()
    }

    var memTop = 0
    val mem = CharArray(MAX_32)

    private fun processExpression() {

        // 첫 번째 숫자(a)는 이미 mem에 있음
        var flags = 0
        val firstNum = String(mem, 0, memTop).toInt()
        memTop = 0

        while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

        if( inputText[vindex] == '&') // & 체크 (상수화 플래그)
        {
            flags += 32
            vindex++
        }

        // b 값 읽기
        var num2 = 0
        while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
            num2 = num2 * 10 + (inputText[vindex] - '0')
            vindex++
        }

        // 공백 건너뛰기
        while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

        // 연산자 확인
        val startCode = when(inputText[vindex]) {
            '+', '-', '*', '/', '%' -> 2.toChar()
            '=', '!', '<', '>' -> 3.toChar()
            else -> 2.toChar()  // 기본값
        }

        val operator = when(inputText[vindex]) {
            '+' -> 1
            '-' -> 2
            '*' -> 3
            '/' -> 4
            '%' -> 5
            '=' -> 1
            '!' -> if (inputText[vindex + 1] == '=') 2 else 0
            '<' -> if (inputText[vindex + 1] == '=') 4 else 3
            '>' -> if (inputText[vindex + 1] == '=') 6 else 5
            else -> 0
        }

        // != <= >= 처리
        if (inputText[vindex + 1] == '=') vindex++
        vindex++

        // 세 번째 숫자(c) 까지 건너뛰기
        while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

        // & 체크 (상수화 플래그)
        if (inputText[vindex] == '&') {
            flags += 64  // 우변 상수화
            vindex++
        }

        // c 값 읽기
        var num3 = 0
        while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
            num3 = num3 * 10 + (inputText[vindex] - '0')
            vindex++
        }
        
        outputChar[top++] = startCode  // 수식 시작
        outputChar[top++] = firstNum.toChar()  // a 값
        outputChar[top++] = num2.toChar() // b값
        outputChar[top++] = num3.toChar() // c값
        outputChar[top++] = (operator + flags).toChar() // 연산자와 플래그 추가

    }

    fun wordRead() {

        firstCase = true

        newlines()

        while (vindex < INPUT_TEXT_MAX && inputText[vindex] != '\u0000') {
            if (top > OUTPUT_TEXT_MAX - 1) break

            val currentChar = inputText[vindex]
            val remainingText = String(inputText, vindex, INPUT_TEXT_MAX - vindex)

            when {
                remainingText.startsWith("case ") -> {
                    // case 키워드 발견시 전용 함수로 처리
                    val endIndex = remainingText.indexOf(':')
                    val caseLine = if (endIndex == -1) remainingText else remainingText.substring(0, endIndex)
                    processCase(caseLine)
                    vindex += caseLine.length
                    continue
                }
                remainingText.startsWith("int ") -> {
                    outputChar[top++] = 6.toChar()  // int 명령어 코드
                    vindex += 3  // "int" 길이

                    // 공백 건너뛰기
                    while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

                    // 첫 번째 숫자 읽기
                    var num1 = 0
                    while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
                        num1 = num1 * 10 + (inputText[vindex] - '0')
                        vindex++
                    }
                    outputChar[top++] = num1.toChar()

                    // 공백 건너뛰기
                    while (vindex < INPUT_TEXT_MAX && inputText[vindex] == ' ') vindex++

                    // 두 번째 숫자 읽기
                    var num2 = 0
                    while (vindex < INPUT_TEXT_MAX && inputText[vindex].isDigit()) {
                        num2 = num2 * 10 + (inputText[vindex] - '0')
                        vindex++
                    }

                    // 두 번째 숫자를 4개의 char로 분리
                    outputChar[top++] = (num2 / 16777216 % 256).toChar()
                    outputChar[top++] = (num2 / 65536 % 256).toChar()
                    outputChar[top++] = (num2 / 256 % 256).toChar()
                    outputChar[top++] = (num2 % 256).toChar()

                    continue
                }
                currentChar == ' ' -> {
                    // 다음 문자들을 확인
                    var nextIndex = vindex + 1
                    while (nextIndex < INPUT_TEXT_MAX && inputText[nextIndex] == ' ') nextIndex++

                    if (nextIndex < INPUT_TEXT_MAX && inputText[nextIndex] == '=') {
                        vindex = nextIndex+1
                        processExpression()  // 수식 처리
                    } else {
                        // 일반적인 공백 처리
                        if (memTop > 0) {
                            val value = String(mem, 0, memTop).toInt()
                            outputChar[top++] = value.toChar()
                            memTop = 0
                        }
                    }
                }
                !currentChar.isDigit() -> {
                    specialKeyCompare(remainingText, INPUT_TEXT_MAX - vindex)
                    wordCompare(currentChar, remainingText, INPUT_TEXT_MAX - vindex)
                }
                else -> {
                    if (memTop < MAX_32) {
                        mem[memTop++] = currentChar
                    }
                }
            }
            vindex++
        }
    }

    fun getOutput(): CharArray = outputChar.copyOf(top)
}