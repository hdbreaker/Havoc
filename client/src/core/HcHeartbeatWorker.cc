#include <Havoc.h>
#include <core/HcHeartbeatWorker.h>

HcHeartbeatWorker::HcHeartbeatWorker()  = default;
HcHeartbeatWorker::~HcHeartbeatWorker() = default;

void HcHeartbeatWorker::run() {
    HeartbeatTimer = new QTimer;

    //
    // connect the updateHeartbeats to the timer
    //
    QObject::connect( HeartbeatTimer, &QTimer::timeout, this, &HcHeartbeatWorker::updateHeartbeats );

    //
    // execute the updater every 800 milliseconds
    //
    HeartbeatTimer->start( 800 );
}

/*!
 * @brief
 *  iterates over all registered agents
 *  and updates the last called time
 */
auto HcHeartbeatWorker::updateHeartbeats() -> void
{
    auto format = QString( "dd-MMM-yyyy HH:mm:ss" );

    //
    // iterate over registered agents
    //
    for ( auto& agent : Havoc->Agents() ) {
        //
        // parse the last called time, calculate the difference,
        // and get seconds, minutes, hours and days from it
        //
        auto last    = QDateTime::fromString( agent->last, format );
        auto current = QDateTime::currentDateTimeUtc();
        auto diff    = last.secsTo( current );
        auto seconds = QDateTime::fromSecsSinceEpoch( diff ).toString( "s" );
        auto minutes = QDateTime::fromSecsSinceEpoch( diff ).toString( "m" );
        auto hours   = QDateTime::fromSecsSinceEpoch( diff ).toString( "h" );
        auto days    = QDateTime::fromSecsSinceEpoch( diff ).toString( "d" );

        //
        // update the table value
        //
        if ( diff < 60 ) {
            agent->ui.Last->setText( QString( "%1s" ).arg( seconds ) );
        } else if ( diff < ( 60 * 60 ) ) {
            agent->ui.Last->setText( QString( "%1m %2s" ).arg( minutes, seconds ) );
        } else if ( diff < 24 * 60 * 60 ) {
            agent->ui.Last->setText( QString( "%1h %2m" ).arg( hours, minutes ) );
        } else {
            agent->ui.Last->setText( QString( "%1d %2h" ).arg( days, hours ) );
        }
    }
}
