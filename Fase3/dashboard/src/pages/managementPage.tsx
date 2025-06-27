import React, { useState } from 'react';
import { FileUploadView } from '../components/FileUploadView';

const Management: React.FC = () => {
    const [activeTab, setActiveTab] = useState<'upload' | 'log'>('upload');

    return (
        <div className="p-6">
            {/* Submenu */}
            <div className="flex border-b border-gray-600 mb-6">
                <button
                    className={`py-2 px-4 font-semibold ${
                        activeTab === 'upload'
                            ? 'border-b-2 border-blue-500 text-blue-500'
                            : 'text-gray-400 hover:text-gray-200'
                    }`}
                    onClick={() => setActiveTab('upload')}
                >
                    Subir archivos
                </button>
                <button
                    className={`ml-6 py-2 px-4 font-semibold ${
                        activeTab === 'log'
                            ? 'border-b-2 border-blue-500 text-blue-500'
                            : 'text-gray-400 hover:text-gray-200'
                    }`}
                    onClick={() => setActiveTab('log')}
                >
                    Registro de cuarentena
                </button>
            </div>

            {/* Contenido según pestaña */}
            {activeTab === 'upload' && (
                <div>
                    <h2 className="text-xl font-semibold mb-4">
                        Vista para subir archivos
                    </h2>
                    <FileUploadView />
                    
                    <p>Aquí implementaremos la subida de archivos.</p>
                </div>
            )}

            {activeTab === 'log' && (
                <div>
                    <h2 className="text-xl font-semibold mb-4">
                        Registro de datos de cuarentena
                    </h2>
                    {/* Aquí iría el componente o contenido para registro */}
                    <p>
                        Aquí implementaremos el registro de datos permitidos y
                        no permitidos.
                    </p>
                </div>
            )}
        </div>
    );
};

export default Management;
