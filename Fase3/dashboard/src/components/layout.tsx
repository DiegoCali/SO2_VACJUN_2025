import { NavLink } from 'react-router-dom';

export default function Layout({ children }: { children: React.ReactNode }) {
    return (
        <div className="min-h-screen flex flex-col bg-gray-900 text-gray-100">
            <header className="bg-gray-800 shadow p-4 flex justify-between items-center">
                <h1 className="text-2xl font-bold">Antivirus Dashboard</h1>
                <nav className="space-x-4">
                    <NavLink
                        to="/info"
                        className={({ isActive }) =>
                            isActive
                                ? 'text-blue-400 font-semibold'
                                : 'text-gray-200 hover:text-blue-400'
                        }
                    >
                        Info
                    </NavLink>
                    <NavLink
                        to="/monitoring"
                        className={({ isActive }) =>
                            isActive
                                ? 'text-blue-400 font-semibold'
                                : 'text-gray-200 hover:text-blue-400'
                        }
                    >
                        Monitoring
                    </NavLink>
                    <NavLink
                        to="/management"
                        className={({ isActive }) =>
                            isActive
                                ? 'text-blue-400 font-semibold'
                                : 'text-gray-200 hover:text-blue-400'
                        }
                    >
                        Management
                    </NavLink>
                </nav>
            </header>

            <main className="p-6 grow">{children}</main>
        </div>
    );
}
