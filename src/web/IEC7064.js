
class IEC7064
{
    alphabet = Array.from('0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ');

    IEC7064()
    {
        //
    }

    /**
     * MOD 37, 36
     */
    checksum(input)
    {
        input = Array.from(input);
        let m = 37;
        let n = 36;
        let produkt = n;
        for(let i = 0; i < input.length; i++)
        {
            let c = this.alphabet.indexOf(input[i]);
            let sum = this.mod((c + produkt), n);
            sum = sum == 0 ? n : sum;
            produkt = this.mod((sum * 2), m);
        }
        let ret = m - produkt;
        return this.alphabet[ret == n ? 0 : ret];
    }

    verify(input)
    {
        input = input.split('');
        console.log(input.slice(0, -1));
        return input.slice(-1)[0] === this.checksum(input.slice(0, -1));
    }

    mod(n, m) {
        return ((n % m) + m) % m;
    }
}
