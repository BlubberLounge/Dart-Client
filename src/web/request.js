
class CRequest
{
    async get(url, cb = null)
    {
        try {
            const response =  await fetch(url, {
                method: "GET",
            });

            const result = await response.json(); // response.text()
            cb(result);
        } catch (err) {
            this.handleError(err);
        }
    }

    async post(url, data, cb = null)
    {
        try {
            const response =  await fetch(url, {
                method: "POST",
                headers: {
                    "Content-Type": "application/x-www-form-urlencoded",
                },
                body: new URLSearchParams(data),
            });

            const result = await response.json(); // response.text()
            cb(result);
        } catch (err) {
            this.handleError(err);
        }
    }

    async postJson(url, data, cb = null)
    {
        try {
            const response =  await fetch(url, {
                method: "POST",
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(data),
            });

            const result = await response.json(); // response.text()
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
