#include "qalsamidibackend.h"
#include "qalsamidideviceinfo.h"

QT_BEGIN_NAMESPACE

QByteArray QAlsaMidiBackend::idToHandle(const snd_seq_addr_t &addr) {
    QByteArray out;
    out.append(reinterpret_cast<const char*>(&addr.client), sizeof(addr.client));
    out.append(reinterpret_cast<const char*>(&addr.port), sizeof(addr.port));
    return out;
}

snd_seq_addr_t QAlsaMidiBackend::handleToId(const QByteArray &handle) {
    snd_seq_addr_t addr;
    if (handle.size() < static_cast<int>(sizeof(addr.client) + sizeof(addr.port))) {
        return addr;
    }
    memcpy(&addr.client, handle.data(), sizeof(addr.client));
    memcpy(&addr.port, handle.data() + sizeof(addr.client), sizeof(addr.port));
    return addr;
}

QAlsaMidiBackend::QAlsaMidiBackend(QMidi::Mode mode, QAlsaMidiPlugin *plugin, unsigned int alsaType)
    : mMode(mode)
    , mPlugin(plugin)
    , mAlsaType(alsaType)
{
}

QAlsaMidiBackend::~QAlsaMidiBackend()
{
}


QList<QByteArray> QAlsaMidiBackend::availableDevices() const
{
    return portHandles(apiData().seq);
}

QString QAlsaMidiBackend::deviceName(const QByteArray& device) const
{
    return getPortName(handleToId(device));
}

QByteArray QAlsaMidiBackend::defaultDevice() const
{
    QList<QByteArray> devices(availableDevices());
    if (devices.size() > 0) {return devices.last();}
    return QByteArray();
}

QList<QByteArray> QAlsaMidiBackend::portHandles(snd_seq_t* seq) const
{
    QList<QByteArray> handles;
    if (!seq) { return handles; }

    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t* pinfo;
    snd_seq_port_info_malloc(&pinfo);
    int client;
    snd_seq_client_info_alloca( &cinfo );

    snd_seq_client_info_set_client( cinfo, -1 );
    while ( snd_seq_query_next_client( seq, cinfo ) >= 0 ) {
        client = snd_seq_client_info_get_client( cinfo );
        if ( client == 0 ) continue;
        // Reset query info
        snd_seq_port_info_set_client( pinfo, client );
        snd_seq_port_info_set_port( pinfo, -1 );
        while ( snd_seq_query_next_port( seq, pinfo ) >= 0 ) {
            unsigned int atyp = snd_seq_port_info_get_type( pinfo );
            if (((atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC) == 0)
                && ((atyp & SND_SEQ_PORT_TYPE_SYNTH) == 0))
                continue;
            unsigned int caps = snd_seq_port_info_get_capability( pinfo );
            if ( ( caps & mAlsaType ) != mAlsaType ) continue;

            auto id = *snd_seq_port_info_get_addr(pinfo);
            auto handle = idToHandle(id);
            QString name = deviceName(handle);
            if (name.contains("QtMidi")) {
                // created by backends
                continue;
            }
            handles.push_back(handle);
        }
    }

    snd_seq_port_info_free(pinfo);

    return handles;
}

QString QAlsaMidiBackend::getPortName(const snd_seq_addr_t &addr) const
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    snd_seq_client_info_alloca( &cinfo );
    snd_seq_port_info_alloca( &pinfo );

    QString stringName;
    if ( QAlsaMidiShared::portInfoById( mApiData.seq, pinfo, mAlsaType, addr ) ) {
        int cnum = snd_seq_port_info_get_client( pinfo );
        snd_seq_get_any_client_info( mApiData.seq, cnum, cinfo );
        stringName.append(snd_seq_client_info_get_name( cinfo ));
        stringName.append(" ");                                    // These lines added to make sure devices are listed
        stringName.append(QString::number(snd_seq_port_info_get_client( pinfo )));  // with full portnames added to ensure individual device names
        stringName.append(":");
        stringName.append(QString::number(snd_seq_port_info_get_port( pinfo )));
        return stringName;
    }

    qWarning() << "QAlsaMidiInBackend::getPortName: error looking for port name!";

    return stringName;
}

QT_END_NAMESPACE
