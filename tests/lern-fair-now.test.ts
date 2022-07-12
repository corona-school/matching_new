import { match } from "../src";
import test from "tape";

/* Queried from Prod on 12-07-2022 with:
query { 
	match_pool(name: "lern-fair-now") {
    pupilsToMatch { 
    	createdAt
      grade: gradeAsInt
      state
      matchRequestCount: openMatchRequestCount
      subjects: subjectsFormatted { name }
    }
    studentsToMatch { 
     	createdAt
      state
      matchRequestCount: openMatchRequestCount
      subjects: subjectsFormatted { name gradeRestriction: grade { min max }} 
    }
  }
}
*/

const DATA = {
    "pupilsToMatch": [
    {
      "createdAt": "2021-06-13T14:39:15.498Z",
      "grade": 8,
      "state": "bb",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Latein"
        }
      ]
    },
    {
      "createdAt": "2021-05-25T16:07:11.108Z",
      "grade": 8,
      "state": "he",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Spanisch"
        }
      ]
    },
    {
      "createdAt": "2021-02-25T13:53:49.728Z",
      "grade": 7,
      "state": "he",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        }
      ]
    },
    {
      "createdAt": "2021-01-17T21:27:37.767Z",
      "grade": 2,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2020-08-09T10:48:48.580Z",
      "grade": 9,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Französisch"
        },
        {
          "name": "Englisch"
        }
      ]
    },
    {
      "createdAt": "2020-11-29T11:32:31.595Z",
      "grade": 3,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch"
        },
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2020-04-02T19:41:39.534Z",
      "grade": 7,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Chemie"
        },
        {
          "name": "Erdkunde"
        },
        {
          "name": "Latein"
        }
      ]
    },
    {
      "createdAt": "2020-04-02T19:41:27.905Z",
      "grade": 8,
      "state": "sh",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik"
        },
        {
          "name": "Chemie"
        },
        {
          "name": "Physik"
        }
      ]
    },
    {
      "createdAt": "2020-12-10T20:46:39.604Z",
      "grade": 8,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Latein"
        }
      ]
    },
    {
      "createdAt": "2021-05-27T16:39:16.843Z",
      "grade": 7,
      "state": "he",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Italienisch"
        }
      ]
    },
    {
      "createdAt": "2020-11-04T21:11:45.834Z",
      "grade": 6,
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        },
        {
          "name": "Deutsch"
        }
      ]
    },
    {
      "createdAt": "2021-05-11T10:00:01.002Z",
      "grade": 3,
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch als Zweitsprache"
        }
      ]
    },
    {
      "createdAt": "2020-12-19T23:00:18.371Z",
      "grade": 3,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch"
        },
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2022-03-25T17:59:51.801Z",
      "grade": 7,
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch"
        },
        {
          "name": "Geschichte"
        }
      ]
    },
    {
      "createdAt": "2022-01-27T16:30:46.976Z",
      "grade": 9,
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch als Zweitsprache"
        },
        {
          "name": "Englisch"
        }
      ]
    },
    {
      "createdAt": "2021-05-10T20:58:43.593Z",
      "grade": 3,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2021-05-03T19:47:21.018Z",
      "grade": 9,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Physik"
        }
      ]
    },
    {
      "createdAt": "2021-06-23T11:39:06.198Z",
      "grade": 7,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        },
        {
          "name": "Französisch"
        },
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2021-06-15T10:40:27.622Z",
      "grade": 6,
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        }
      ]
    },
    {
      "createdAt": "2021-01-13T11:44:58.630Z",
      "grade": 8,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2021-04-16T13:22:46.579Z",
      "grade": 3,
      "state": "bw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch als Zweitsprache"
        }
      ]
    },
    {
      "createdAt": "2022-03-15T19:31:33.081Z",
      "grade": 9,
      "state": "ni",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch"
        },
        {
          "name": "Deutsch als Zweitsprache"
        },
        {
          "name": "Kunst"
        }
      ]
    },
    {
      "createdAt": "2021-01-04T13:45:59.827Z",
      "grade": 13,
      "state": "hh",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Biologie"
        },
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2022-03-31T15:23:00.372Z",
      "grade": 2,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch als Zweitsprache"
        },
        {
          "name": "Mathematik"
        },
        {
          "name": "Deutsch"
        }
      ]
    },
    {
      "createdAt": "2021-05-26T19:59:12.517Z",
      "grade": 4,
      "state": "be",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        },
        {
          "name": "Deutsch"
        },
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2020-09-07T18:58:24.764Z",
      "grade": 12,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik"
        }
      ]
    },
    {
      "createdAt": "2022-01-27T17:11:29.479Z",
      "grade": 10,
      "state": "sh",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Chemie"
        },
        {
          "name": "Physik"
        }
      ]
    },
    {
      "createdAt": "2021-05-06T17:48:11.115Z",
      "grade": 6,
      "state": "be",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch als Zweitsprache"
        }
      ]
    },
    {
      "createdAt": "2021-03-14T19:42:26.164Z",
      "grade": 2,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Deutsch"
        },
        {
          "name": "Mathematik"
        },
        {
          "name": "Deutsch als Zweitsprache"
        }
      ]
    },
    {
      "createdAt": "2020-07-22T06:01:05.495Z",
      "grade": 5,
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch"
        }
      ]
    }
  ],
  "studentsToMatch": [
    {
      "createdAt": "2020-04-05T11:52:38.421Z",
      "state": "ni",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Biologie",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        },
        {
          "name": "Chemie",
          "gradeRestriction": {
            "min": 1,
            "max": 9
          }
        },
        {
          "name": "Mathematik",
          "gradeRestriction": {
            "min": 1,
            "max": 9
          }
        }
      ]
    },
    {
      "createdAt": "2022-04-15T11:52:56.949Z",
      "state": "sl",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik",
          "gradeRestriction": {
            "min": 5,
            "max": 10
          }
        },
        {
          "name": "Physik",
          "gradeRestriction": {
            "min": 5,
            "max": 10
          }
        }
      ]
    },
    {
      "createdAt": "2020-09-10T21:31:44.703Z",
      "state": "be",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Informatik",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        }
      ]
    },
    {
      "createdAt": "2022-07-04T07:42:29.238Z",
      "state": "nw",
      "matchRequestCount": 2,
      "subjects": [
        {
          "name": "Physik",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        },
        {
          "name": "Kunst",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        },
        {
          "name": "Englisch",
          "gradeRestriction": {
            "min": 1,
            "max": 10
          }
        },
        {
          "name": "Deutsch",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        },
        {
          "name": "Mathematik",
          "gradeRestriction": {
            "min": 1,
            "max": 10
          }
        },
        {
          "name": "Deutsch als Zweitsprache",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        }
      ]
    },
    {
      "createdAt": "2022-06-14T12:08:23.546Z",
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Wirtschaft",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        }
      ]
    },
    {
      "createdAt": "2022-07-05T16:31:12.990Z",
      "state": "by",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik",
          "gradeRestriction": {
            "min": 10,
            "max": 13
          }
        }
      ]
    },
    {
      "createdAt": "2022-02-26T00:48:27.706Z",
      "state": "nw",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Mathematik",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        },
        {
          "name": "Physik",
          "gradeRestriction": {
            "min": 1,
            "max": 10
          }
        },
        {
          "name": "Informatik",
          "gradeRestriction": {
            "min": 1,
            "max": 10
          }
        }
      ]
    },
    {
      "createdAt": "2022-07-09T05:20:53.130Z",
      "state": "st",
      "matchRequestCount": 1,
      "subjects": [
        {
          "name": "Englisch",
          "gradeRestriction": {
            "min": 1,
            "max": 13
          }
        }
      ]
    }
]
};

for (const [index, pupil] of DATA.pupilsToMatch.entries()) {
    (pupil as any).excludeMatchesWith = [];
    (pupil as any).id = index;
    (pupil as any).uuid = "Helpee" + index;
}

for (const [index, student] of DATA.studentsToMatch.entries()) {
    (student as any).excludeMatchesWith = [];
    (student as any).id = index;
    (student as any).uuid = "Helpee" + index;
}

test(`Matching Lern-Fair Now Sample`, t => {
    match(DATA.studentsToMatch as any, DATA.pupilsToMatch as any);
    t.end();
});