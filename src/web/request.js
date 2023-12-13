
class CRequest
{
    async get(url, data, cb = null)
    {
        await this.send(url, 'GET', data, cb);
    }

    async post(url, data, cb = null)
    {
        await this.send(url, 'POST', data, cb);
    }

    async send(url, method, data, cb = null)
    {
        try {
            const response =  await fetch(url, {
                method: method,
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded",
                },
                body: new URLSearchParams(data),
            });

            const result = await response.json(); // response.test()
            cb(result);
        } catch (err) {
            this.handleError(err);
        }
    }

    handleError(err)
    {
        console.error(err);
    }
}
