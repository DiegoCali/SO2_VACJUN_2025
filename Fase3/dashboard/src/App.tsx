import { useState } from 'react';
import InfoPage from './pages/infoPage';
import MonitoringPage from './pages/monitoringPage';
import ManagementPage from './pages/managementPage';
import Layout from './components/layout';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';

function App() {
    return (
        <Router>
            <Layout>
                <Routes>
                    <Route path="/" element={<Navigate to="/info" replace />} />
                    <Route path="/info" element={<InfoPage />} />
                    <Route path="/monitoring" element={<MonitoringPage />} />
                    <Route path="/management" element={<ManagementPage />} />
                </Routes>
            </Layout>
        </Router>
    );
}

export default App;
