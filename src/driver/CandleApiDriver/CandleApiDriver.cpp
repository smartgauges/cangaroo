/*

  Copyright (c) 2016 Hubert Denkmair <hubert@denkmair.de>

  This file is part of cangaroo.

  cangaroo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  cangaroo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with cangaroo.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "CandleApiDriver.h"
#include "api/candle.h"

#include "CandleApiInterface.h"
#include <driver/GenericCanSetupPage.h>

CandleApiDriver::CandleApiDriver(Backend &backend)
  : CanDriver(backend),
    setupPage(new GenericCanSetupPage(0))
{
    QObject::connect(&backend, SIGNAL(onSetupDialogCreated(SetupDialog&)), setupPage, SLOT(onSetupDialogCreated(SetupDialog&)));
}

QString CandleApiDriver::getName()
{
    return "CandleAPI";
}

bool CandleApiDriver::update()
{
    deleteAllInterfaces();

    candle_list_handle clist;
    uint8_t num_interfaces;
    candle_handle dev;

    if (candle_list_scan(&clist)) {
        if (candle_list_length(clist, &num_interfaces)) {
            for (uint8_t i=0; i<num_interfaces; i++) {
                if (candle_dev_get(clist, i, &dev)) {
                    candle_devstate_t state;
                    if (!candle_dev_get_state(dev, &state))
                        continue;

                    if (state != CANDLE_DEVSTATE_AVAIL)
                        continue;

                    uint8_t num_channels = 0;
                    candle_channel_count(dev, &num_channels);

                    for (uint8_t c = 0; c < num_channels; c++) {

                        CandleApiInterface *cif = findInterface(dev, c);

                        if (cif == NULL) {
                            cif = new CandleApiInterface(this, dev, c);
                            addInterface(cif);
                        }
                        else {
                            cif->update(dev);
                        }
                    }
                }
            }
        }
        candle_list_free(clist);
    }

    return true;
}

CandleApiInterface *CandleApiDriver::findInterface(candle_handle dev, uint8_t channel)
{
    foreach (CanInterface *intf, getInterfaces()) {
        CandleApiInterface *cif = dynamic_cast<CandleApiInterface*>(intf);
        if ((cif->getPath() == std::wstring(candle_dev_get_path(dev))) && (cif->getChannel() == channel)) {
            return cif;
        }
    }
    return NULL;
}
