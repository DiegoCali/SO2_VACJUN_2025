import React, { useEffect, useState } from 'react';
import { toast, ToastContainer } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import { post, get } from '../services/api';

type FileItem = {
    name: string;
};

export const QuarantineFilesView: React.FC = () => {
    const [files, setFiles] = useState<FileItem[]>([]);
    const [loading, setLoading] = useState(false);

    const fetchFiles = async () => {
        setLoading(true);
        try {
            const data = await get<{ files: FileItem[] }>(
                '/quarantine_files'
            );
            setFiles(data.files);
        } catch (error) {
            toast.error('Error al cargar archivos en cuarentena');
            console.error(error);
        }
        setLoading(false);
    };

    useEffect(() => {
        fetchFiles();
    }, []);

    const handleRestore = async (filename: string) => {
        try {
            const res = await post('/restore_file', { filename });
            if (res.status === 200) {
                toast.success(`Archivo restaurado: ${res.filename}`);
                fetchFiles();
            } else {
                toast.error(`Error al restaurar: ${res.filename}`);
            }
        } catch (error) {
            toast.error('Error en la solicitud de restauración');
            console.error(error);
        }
    };

    const handleDelete = async (filename: string) => {
        try {
            const res = await post('/delete_file', { filename });
            if (res.status === 200) {
                toast.success(`Archivo eliminado: ${res.filename}`);
                fetchFiles();
            } else {
                toast.error(`Error al eliminar: ${res.filename}`);
            }
        } catch (error) {
            toast.error('Error en la solicitud de eliminación');
            console.error(error);
        }
    };

    return (
        <div className="max-w-4xl mx-auto p-6 bg-gray-850 rounded-xl shadow-md text-gray-100">
            <h2 className="text-xl font-semibold mb-4">
                Archivos en Cuarentena
            </h2>
            <button
                onClick={fetchFiles}
                disabled={loading}
                className="mb-4 bg-blue-900 text-white px-4 py-2 rounded hover:bg-blue-700 disabled:opacity-50"
            >
                {loading ? 'Cargando...' : 'Actualizar Lista'}
            </button>

            <table className="w-full table-auto border-collapse border border-gray-700">
                <thead>
                    <tr className="bg-gray-800">
                        <th className="border border-gray-700 px-4 py-2 text-left">
                            Nombre del archivo
                        </th>
                        <th className="border border-gray-700 px-4 py-2">
                            Acciones
                        </th>
                    </tr>
                </thead>
                <tbody>
                    {files.length === 0 ? (
                        <tr>
                            <td
                                colSpan={2}
                                className="text-center py-4 text-gray-400"
                            >
                                No hay archivos en cuarentena
                            </td>
                        </tr>
                    ) : (
                        files.map(({ name }) => (
                            <tr key={name} className="hover:bg-gray-800">
                                <td className="border border-gray-700 px-4 py-2 break-all">
                                    {name}
                                </td>
                                <td className="border border-gray-700 px-4 py-2 space-x-2 text-center">
                                    <button
                                        onClick={() => handleRestore(name)}
                                        className="bg-green-700 hover:bg-green-800 text-white px-3 py-1 rounded"
                                    >
                                        Restaurar
                                    </button>
                                    <button
                                        onClick={() => handleDelete(name)}
                                        className="bg-red-700 hover:bg-red-800 text-white px-3 py-1 rounded"
                                    >
                                        Eliminar
                                    </button>
                                </td>
                            </tr>
                        ))
                    )}
                </tbody>
            </table>

            <ToastContainer
                position="top-right"
                autoClose={3000}
                theme="dark"
            />
        </div>
    );
};
