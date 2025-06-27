import React, { useState } from 'react';
import { post } from '../services/api';

export const FileUploadView: React.FC = () => {
    const [filePath, setFilePath] = useState('');
    const [feedback, setFeedback] = useState('');

    const handleSubmit = async (type: 'scan' | 'signature') => {
        if (!filePath.trim()) {
            setFeedback('Por favor, ingresa una ruta de archivo válida.');
            return;
        }

        const endpoint =
            type === 'scan' ? '/scan_file' : '/add_signature';

        try {
            const res = await post(endpoint, {
                file_path: filePath.trim(),
                hash: '',
            });

            setFeedback(
                `${type === 'scan' ? 'Escaneo' : 'Firma'} completado para ${res.file_path}`
            );
        } catch (err) {
            setFeedback(
                `Error al ${type === 'scan' ? 'escanear' : 'firmar'} el archivo.`
            );
            console.error(err);
        }
    };

    return (
        <div className="bg-gray-50 p-8 rounded-xl shadow-lg max-w-xl mx-auto">
            <h2 className="text-2xl font-bold mb-6 text-gray-800 text-center">
                Subir Archivo
            </h2>

            <label
                htmlFor="filePath"
                className="block mb-2 text-gray-700 font-medium"
            >
                Ruta del archivo:
            </label>
            <input
                id="filePath"
                type="text"
                value={filePath}
                onChange={(e) => setFilePath(e.target.value)}
                placeholder="Escribe o pega la ruta completa del archivo"
                className="w-full px-4 py-2 mb-6 rounded border border-gray-300 focus:outline-none focus:ring-2 focus:ring-blue-400 text-gray-900"
            />

            <div className="flex justify-center gap-6">
                <button
                    onClick={() => handleSubmit('signature')}
                    disabled={!filePath.trim()}
                    className={`px-6 py-2 rounded font-semibold text-white ${
                        filePath.trim()
                            ? 'bg-green-600 hover:bg-green-700'
                            : 'bg-green-300 cursor-not-allowed'
                    }`}
                >
                    Agregar Firma
                </button>

                <button
                    onClick={() => handleSubmit('scan')}
                    disabled={!filePath.trim()}
                    className={`px-6 py-2 rounded font-semibold text-white ${
                        filePath.trim()
                            ? 'bg-indigo-600 hover:bg-indigo-700'
                            : 'bg-indigo-300 cursor-not-allowed'
                    }`}
                >
                    Escanear Archivo
                </button>
            </div>

            {feedback && (
                <p className="mt-6 text-center text-gray-700 font-medium">
                    {feedback}
                </p>
            )}
        </div>
    );
};
