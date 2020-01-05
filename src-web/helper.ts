/**
 * Parse key-value separate by tab
 */
export function parseResponse<T = any>(res: string): T {
    const result = {} as any;
    res.split("\n").forEach(
        s => {
            const [key, value] = s.split("\t", 2);
            if ('undefined' == typeof result[key]) {
                result[key] = value
            } else {
                result[key] = Array.isArray(result[key]) ? result[key] : [result[key]]
                result[key].push(value)
            }
        }
    )
    return result;
}
