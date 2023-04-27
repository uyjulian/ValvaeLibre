'use strict';

const puppeteer = require('puppeteer');

(async () => {
  const browser = await puppeteer.launch();
  const page = await browser.newPage();

  try
  {
    await page.goto('file://' + __dirname + "/../html/camshaft.html");

    // Set screen size
    await page.setViewport({width: 1080, height: 1024});

    const selectors = [
      ['#num-cyl', 'select', '1'],
      ['#in_lift_valve', 'type', '2'],
      ['#ex_lift_valve', 'type', '2'],
      ['#in_duration', 'type', '2'],
      ['#ex_duration', 'type', '2'],
      ['#in_open', 'type', '2'],
      ['#specifier1', 'select', 'abdc'],
      ['#ex_open', 'type', '2'],
      ['#specifier3', 'select', 'abdc'],
      ['#in_close', 'type', '2'],
      ['#specifier2', 'select', 'bbdc'],
      ['#ex_close', 'type', '2'],
      ['#specifier4', 'select', 'btdc'],
    ];

    for (const curval of selectors)
    {
      if (curval[1] === "type")
      {
        await page.$eval(curval[0], el => el.value = '');
        await page.type(curval[0], curval[2]);
      }
      else if (curval[1] == "select")
      {
        await page.select(curval[0], curval[2]);
      }
    }

    await page.screenshot({path: 'out.png'});

  }
  catch (e)
  {

  }

  await browser.close();
})();
