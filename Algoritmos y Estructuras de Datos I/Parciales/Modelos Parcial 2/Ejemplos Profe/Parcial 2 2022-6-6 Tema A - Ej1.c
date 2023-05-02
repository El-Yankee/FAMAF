#include <assert.h>
#include <stdio.h>





int main(void) {
    int r, s;

    printf("Ingrese un valor para r: ");
    scanf("%d", &r);
    printf("Ingrese un valor para s: ");
    scanf("%d", &s);

    int R = r, S = s;

    assert(R == r && S == s && S > R);

    // r, s := s - r, r + s

    r = S - R;
    s = R + S;

    assert(r == S - R && s == R + S);

    printf("r = %d, s = %d\n", r, s);

    return 0;
}









