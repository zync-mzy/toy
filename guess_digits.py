# -*- coding: UTF-8 -*-

def getHint(secret, guess):
    """
    :type secret: str
    :type guess: str
    :rtype: str
    """
    bA = []
    bB = []
    iA = 0
    iB = 0
    A = 0
    B = 0
    while iA < len(secret) and iB < len(guess):
        vA = secret[iA]
        vB = guess[iB]
        iA += 1
        iB += 1
        
        if vA == vB:
            A += 1
            continue
            
        bA.append(vA)
        bB.append(vB)
        if vA in bB:
            B += 1
            bB.remove(vA)
            bA.remove(vA)
        if vB in bA:
            B += 1
            bA.remove(vB)
            bB.remove(vB)
            
    return "%dA%dB" % (A, B)

def genSecret():
    import random

    digits = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    length = 4
    secret = ""
    for i in range(length):
        val = digits[int(random.random() * len(digits))]
        digits.remove(val)
        secret += str(val)
    return secret

import sys

print '''
欢迎参加猜数字小游戏！
游戏规则如下：
1.系统会随机生成四个数字（0-9），每个数字均不相同
2.你的任务是通过系统提示猜出这四个数字
3.系统会先要求你输入你的猜测，然后通过A和B的数量来提示你猜测的准确性
4.A表示数字正确且位置正确，B表示数字正确但位置不正确
  比如系统生成了1234，你的猜测是5432，那么系统会提示1A2B
  其中1A表示3的数字和位置均正确，2B表示2和4虽然数字正确但是位置不正确
5.你只有十次机会

让我们开始吧！
'''

secret = genSecret()
times = 10
guess = ""
cnt = 0
while cnt < times:
    cnt += 1
    guess = raw_input("The %dth guess: " % cnt)
    if guess == secret:
        break
    print "Hints: %s" % getHint(secret, guess)

if guess == secret:
    print "\nThat's it! Congras!"
else:
    print "\nThe answer is %s" % secret
