def checkPals(s: str, subs: list[str]) -> list[list[int]]:
    base = 31
    mod = int(1e9)+7
    pows = [0]*(len(s)+1)
    pows[0] = 1
    for i in range(1, len(pows)):
        pows[i] = (pows[i-1]*base)%mod

    # Hashcodes of all elements of sub, separated by parity
    hs = [set(), set()]
    for sub in subs:
        hashCode = 0
        for i in range(int(len(sub)/2), len(sub)):
            charVal = ord(sub[i])-ord('a')+1
            hashCode = (hashCode*base+charVal)%mod
        hs[len(sub)&1].add(hashCode)

    # Insert a space between all characters and a special character at front and end
    # Example: "abc" -> "< a b c >"
    arr = [0] * (2*len(s)+3)
    for i in range(len(s)):
        arr[2*i+2] = ord(s[i])-ord('a')+1
    arr[0] = -1
    arr[len(arr)-1] = -2

    # Length of longest (not necessarily valid) palindrome centered at i
    dp = [0] * (2*len(s)+3)

    # Hashcode of longest (not necessarily valid) palindrome centered at i
    codes = [0] * (2*len(s)+3)

    # Length of first invalid palindrome centered at i
    bounds = [(2*len(s)+3) for _ in range((2*len(s)+3))]
    left, right = 0, 0

    # Running hashcode of interval [i, right]
    hashCode = 0

    # Manacher's algorithm with the codes and bounds arrays piggybacking
    # off of the string traversal/mirroring
    for i in range(2, len(dp)-2):
        if (i+1 >= right):
            codes[i] = arr[i]
        elif (right-i < dp[right-i+left]):
            dp[i] = right-i
            codes[i] = hashCode if arr[i] == 0 else hashCode*base+arr[i]
            if (right-i > bounds[right-i+left]): 
                bounds[i] = bounds[right-i+left]
        else:
            dp[i] = dp[right-i+left]
            codes[i] = codes[right-i+left]
            bounds[i] = bounds[right-i+left]

        while arr[i - dp[i] - 1] == arr[i + dp[i] + 1]:
            if (arr[i - dp[i] - 1] != 0): 
                codes[i] = (codes[i] * base + arr[i - dp[i] - 1]) % mod

            # If current hashcode contained in subs, current palindrome is invalid
            if (codes[i] in hs[1^i&1]): 
                bounds[i] = min(bounds[i], dp[i])

            dp[i] += 1


        if i+dp[i] > right:
            right = i+dp[i]
            left = 2*i-right
            hashCode = codes[i]

        if (arr[i] != 0):
            hashCode = (hashCode-arr[i]*pows[int((right-i)/2)]+base*mod)%mod

    # Rightmost index whose inclusion would necessarily mean containing an invalid palindrome
    bound = 0
    for i in range(2, len(dp)-2):
        bound = max(bound, i - bounds[i])
        dp[i] = min(dp[i], i - bound)

    ans = [[dp[i*2+2], dp[i*2+3]] for i in range(len(s))]

    return ans