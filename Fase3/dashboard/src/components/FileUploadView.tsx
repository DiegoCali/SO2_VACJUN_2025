import React, { useState } from 'react';
import { post } from '../services/api';

// Asegura que TypeScript reconozca la API expuesta por Electron
declare global {
  interface Window {
    electronAPI?: {
      selectFile: () => Promise<string | null>;
    };
  }
}

export const FileUploadView: React.FC = () => {
  const [filePath, setFilePath] = useState<string | null>(null);
  const [feedback, setFeedback] = useState('');

  const handleSelectFile = async () => {
    try {
      const path = await window.electronAPI?.selectFile();
      if (path) {
        setFilePath(path);
        setFeedback('');
      }
    } catch (error) {
      console.error('Error al seleccionar archivo:', error);
      setFeedback('No se pudo seleccionar el archivo.');
    }
  };

  const handleSubmit = async (type: 'scan' | 'signature') => {
    if (!filePath) return;

    const endpoint = type === 'scan' ? '/api/scan_file' : '/api/add_signature';

    try {
      const res = await post(endpoint, {
        file_path: filePath,
        hash: '',
      });

      setFeedback(
        `${type === 'scan' ? 'Escaneo' : 'Firma'} completado para ${res.file_path}`
      );
    } catch (err) {
      setFeedback(`Error al ${type === 'scan' ? 'escanear' : 'firmar'} el archivo`);
      console.error(err);
    }
  };

  return (
    <div className="bg-gray-100 p-6 rounded-xl shadow-md w-full max-w-2xl mx-auto">
      <h2 className="text-xl font-semibold mb-4">Subir Archivo</h2>

      <div className="mb-4">
        <button
          className="bg-blue-600 text-white px-4 py-2 rounded hover:bg-blue-700"
          onClick={handleSelectFile}
        >
          Seleccionar archivo
        </button>
        {filePath && (
          <p className="mt-3 text-sm text-gray-600 break-all">
            Archivo seleccionado:{' '}
            <span className="font-medium">{filePath}</span>
          </p>
        )}
      </div>

      <div className="flex gap-4">
        <button
          className="bg-green-600 hover:bg-green-700 text-white font-semibold py-2 px-4 rounded"
          onClick={() => handleSubmit('signature')}
          disabled={!filePath}
        >
          Agregar Firma
        </button>
        <button
          className="bg-indigo-600 hover:bg-indigo-700 text-white font-semibold py-2 px-4 rounded"
          onClick={() => handleSubmit('scan')}
          disabled={!filePath}
        >
          Escanear Archivo
        </button>
      </div>

      {feedback && (
        <p className="mt-4 text-sm text-gray-700">{feedback}</p>
      )}
    </div>
  );
};
