// src/types/global.d.ts
export {};

declare global {
    interface Window {
        electronAPI?: {
            selectFile: () => Promise<string | null>;
        };
    }
}
