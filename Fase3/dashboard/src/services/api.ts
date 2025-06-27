const API_BASE = 'http://localhost:5000/api';

type JSONValue =
    | string
    | number
    | boolean
    | null
    | JSONValue[]
    | { [key: string]: JSONValue };

export async function get<T = any>(endpoint: string): Promise<T> {
    const response = await fetch(`${API_BASE}${endpoint}`);
    if (!response.ok) {
        throw new Error(`GET ${endpoint} failed: ${response.status}`);
    }
    return await response.json();
}

export async function post<T = any>(
    endpoint: string,
    body: Record<string, JSONValue>
): Promise<T> {
    const response = await fetch(`${API_BASE}${endpoint}`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify(body),
    });
    if (!response.ok) {
        throw new Error(`POST ${endpoint} failed: ${response.status}`);
    }
    return await response.json();
}
